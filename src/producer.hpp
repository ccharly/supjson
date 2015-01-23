#ifndef SUPJSON_JSON_PRODUCER__
# define SUPJSON_JSON_PRODUCER__

# include <istream>
# include <sstream>
# include <fstream>
# include <stdexcept>

# include "exceptions.hpp"

namespace supjson {

    /* A producer produces input. You can either peek the current byte (NOTE:
     * this action won't move the cursor of the internal buffer) or you can
     * next it.
     *
     * See implementation for more details.
     * */
    template <typename T>
    struct basic_producer {
        public:
            typedef std::basic_istream<T> istream;
            typedef std::basic_istringstream<T> istringstream;

		private:
			size_t line_, column_;

        // Helpers to construct a producer
        public:
            static basic_producer from_file(std::string const& f)
            {
                return basic_producer(new std::ifstream(f, std::ifstream::in));
            }

            static basic_producer from_string(std::string const& f)
            {
                return basic_producer(new std::istringstream(f));
            }

            static basic_producer from_streambuf(std::streambuf* sb)
            {
                return basic_producer(new std::istream(sb));
            }

        // You can only construct a fresh new producer using the helpers
        // functions below
        private:
            basic_producer(istream* is)
                : is_(is), peeked_(0), has_peeked_(false)
            {
            }

        // Also, the move constructor is available
        public:
            basic_producer(basic_producer&& p)
                : is_(p.is_), peeked_(0), has_peeked_(false)
            {
                p.is_ = 0;
            }

            ~basic_producer()
            {
                if (is_)
                    delete is_;
            }

		public:
			bool good() const
			{
				return is_->good();
			}

        private:
            T getc__()
            {
				char c = 0;

				if (skip_ws_) {
					do {
						*is_ >> c;
						if (c == '\n') {
							line_ += 1;
							column_ = 0;
						} else {
							column_ += 1;
						}
					} while (c == ' ' || c == '\t' || c == '\n');
				} else {
					*is_ >> std::noskipws >> c;
					if (c == '\n') {
						line_ += 1;
						column_ = 0;
					} else {
						column_ += 1;
					}
				}

                return c;
            }

            T peekc__()
            {
                T c;
                if (has_peeked_) {
                    c = peeked_;
                } else {
					c = getc__();
                    peeked_ = c;
                    has_peeked_ = true;
                }
                return c;
            }

        public:
			size_t get_line() const
			{
				return line_;
			}

			size_t get_column() const
			{
				return column_;
			}

			void skip_ws(bool skip)
			{
				skip_ws_ = skip;
			}

            // Returns the current read character
            T peekc()
            {
                if (eof()) {
                    throw end_of_file();
                }

                return peekc__();
            }

            // Returns the current read character and next to the following one
            T nextc()
            {
                if (eof()) {
                    throw end_of_file();
                }

                T c;
                if (has_peeked_) {
                    has_peeked_ = false;
                    c = peeked_;
                } else {
					c = getc__();
                }
                return c;
            }

            // Returns true if EOF has been reached
            bool eof()
            {
                /* Here we do a fake peek, to avoid the lack of doing
                 * an extra nextc to reach EOF:
                 *
                 * DETAIL:
                 *  let assume S = "salut", when the cursor will be positionned
                 *  on the 't', you have to do an extra read to reach EOF.
                 *
                 * Using an extra internal read, allow the final user to get
                 * only valid characters.
                 *
                 * */
                (void)peekc__();
                return is_->eof();
            }

        private:
            istream* is_;
            T peeked_; // FIXME could be replaced by an optional type
            bool has_peeked_;
			bool skip_ws_;
    };

    // Typedefs
    typedef basic_producer<char> producer;
}

#endif
