#pragma once

#include <core/types.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class String
{
private:
    char* _data;   // Gerenciamento dinâmico de memória (char normal, não const**)
    usize _size;   // Guarda o tamanho do texto (sem contar o \0)

public:
    // Construtores e Destrutor
    String() { init(""); }
    String(const char* cstr) { init(cstr); }
    String(const String& str) { init(str.data()); }
    ~String() { clean(); }

    // Getters
    const char* data() const { return _data; }
    usize size() const { return _size; }

    // Modificadores (Append)
    void append(const char* new_data)
    {
        if (!new_data) return;
        usize new_len = strlen(new_data);
        usize total_len = _size + new_len;

        char* buffer = new char[total_len + 1];
        strcpy(buffer, _data);
        strcat(buffer, new_data);

        clean();
        _data = buffer;
        _size = total_len;
    }

    void append(const String& new_data)
    {
        append(new_data.data());
    }

    // Operações de Texto
    void join(const char* delimiter, const String* strings, usize count)
    {
        clean();
        init("");
        for (usize i = 0; i < count; i++)
        {
            append(strings[i].data());
            if (i < count - 1 && delimiter)
            {
                append(delimiter);
            }
        }
    }

    void reverse()
    {
        if (_size <= 1) return;
        for (usize i = 0; i < _size / 2; i++)
        {
            char temp = _data[i];
            _data[i] = _data[_size - 1 - i];
            _data[_size - 1 - i] = temp;
        }
    }

    String& trim()
    {
        if (_size == 0) return *this;

        usize start = 0;
        while (start < _size && (_data[start] == ' ' || _data[start] == '\t' || _data[start] == '\n' || _data[start] == '\r')) {
            start++;
        }

        usize end = _size;
        while (end > start && (_data[end - 1] == ' ' || _data[end - 1] == '\t' || _data[end - 1] == '\n' || _data[end - 1] == '\r')) {
            end--;
        }

        usize new_len = end - start;
        char* buffer = new char[new_len + 1];
        memcpy(buffer, &_data[start], new_len);
        buffer[new_len] = '\0';

        clean();
        _data = buffer;
        _size = new_len;
        return *this;
    }

    void no_whitespace()
    {
        char* buffer = new char[_size + 1];
        usize j = 0;
        for (usize i = 0; i < _size; i++)
        {
            if (_data[i] != ' ' && _data[i] != '\t' && _data[i] != '\n' && _data[i] != '\r')
            {
                buffer[j++] = _data[i];
            }
        }
        buffer[j] = '\0';

        clean();
        _data = buffer;
        _size = j;
    }

    // Busca e Contagem
    int count_character(char character) const
    {
        int count = 0;
        for (usize i = 0; i < _size; i++)
        {
            if (_data[i] == character) count++;
        }
        return count;
    }

    bool have_character(const char character) const
    {
        for (usize i = 0; i < _size; i++)
        {
            if (_data[i] == character) return true;
        }
        return false;
    }

    u32 hash() const
    {
        u32 hash = 5381; // Algoritmo DJB2
        for (usize i = 0; i < _size; i++)
        {
            hash = ((hash << 5) + hash) + _data[i];
        }
        return hash;
    }

    // Arquivos (I/O)
    bool save(const char* path) const
    {
        FILE* file = fopen(path, "w");
        if (!file) return false;
        fprintf(file, "%s", _data);
        fclose(file);
        return true;
    }

    bool load(const char* path)
    {
        FILE* file = fopen(path, "r");
        if (!file) return false;

        fseek(file, 0, SEEK_END);
        long length = ftell(file);
        fseek(file, 0, SEEK_SET);

        if (length < 0) { fclose(file); return false; }

        char* buffer = new char[length + 1];
        size_t read_bytes = fread(buffer, 1, length, file);
        buffer[read_bytes] = '\0';

        clean();
        _data = buffer;
        _size = read_bytes;

        fclose(file);
        return true;
    }

    // Operadores
    bool operator==(const String& other) const
    {
        if (_size != other._size) return false;
        return strcmp(_data, other._data) == 0;
    }

    String operator+(const String& other) const
    {
        String result(*this);
        result.append(other);
        return result;
    }

    String& operator+=(const String& other)
    {
        this->append(other);
        return *this;
    }

    String& operator=(const String& other)
    {
        if (this != &other)
        {
            clean();
            init(other.data());
        }
        return *this;
    }

private:
    void init(const char* data)
    {
        if (!data) data = "";
        _size = strlen(data);
        _data = new char[_size + 1];
        strcpy(_data, data);
    }

    void clean()
    {
        delete[] _data;
        _data = nullptr;
    }
};
