typedef struct
{
    char* c_str;
    s64 len;
    s64 cap;
} string;

INTERNAL string* make_string(char* str);
INTERNAL string* make_string_f(char* fmt, ...);
INTERNAL void string_destroy(string* this);
INTERNAL char* string_data(string* this);
INTERNAL void string_append(string* this, char* str);
INTERNAL void string_append_f(string* this, char* fmt, ...);

INTERNAL string* make_string(char* str)
{
    xassert(str);
    string* s = xmalloc(sizeof(string));
    s->len = 0;
    s->cap = 16;
    s->c_str = xmalloc(s->cap);
    string_append(s, str);
    return s;
}
INTERNAL string* make_string_f(char* fmt, ...)
{
    xassert(fmt);
    va_list args;
    va_start(args, fmt);
    char* tmp = vstrf(fmt, args);
    va_end(args);
    string* s = make_string(tmp);
    free(tmp);
    return s;
}
INTERNAL char* string_data(string* this)
{
    xassert(this);
    xassert(this->c_str);
    return this->c_str;
}
INTERNAL void string_append(string* this, char* str)
{
    xassert(this);
    s64 str_len = strlen(str);
    xassert(this->len <= this->cap);
    while (this->len + str_len + 1 >= this->cap) {
        this->cap *= 2;
        this->c_str = xrealloc(this->c_str, this->cap * sizeof(*this->c_str));
    }
    memcpy(this->c_str + this->len, str, str_len);
    this->len += str_len;
    this->c_str[this->len] = '\0';
}
INTERNAL void string_append_f(string* this, char* fmt, ...)
{
    xassert(this);
    xassert(fmt);
    va_list args;
    va_start(args, fmt);
    char* tmp = vstrf(fmt, args);
    va_end(args);
    string_append(this, tmp);
    free(tmp);
}
INTERNAL void string_destroy(string* this)
{
    xassert(this);
    free(this->c_str);
    free(this);
}
