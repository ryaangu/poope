#ifndef CORE_HASH_MAP_HPP
#define CORE_HASH_MAP_HPP

#include <core/system.hpp>
#include <core/hash.hpp>
#include <core/compare.hpp>
#include <initializer_list>

// Hash Map Structure
template<typename Key, typename Value> struct HashMap
{
    // Entry
    struct Entry
    {
        Key   key;
        Value value;
        int   used;
    };

    // Entry Initializer
    struct EntryInitializer
    {
        Key   key;
        Value value;
    };

    int    size;
    int    room;
    Entry *data;

    // Initialize
    inline void init(int capacity = 64)
    {
        size = 0;
        room = capacity;

        data = reinterpret_cast<Entry *>(memory_allocate(room * sizeof(Entry)));

        // Fill data with empty entries
        for (int i = 0; i < room; ++i)
            data[i].used = false;
    }

    // Constructors
    inline HashMap() {};

    inline HashMap(const std::initializer_list<EntryInitializer> &entries)
    {
        // Initialize
        const int length = static_cast<int>(entries.size()) + 10;
        init(length);

        // Set keys and values
        for (const EntryInitializer &entry : entries)
            set(entry.key, entry.value);
    }

    // Find Entry
    inline Entry *find_entry(Entry *entries, const Key &key)
    {
        // Hash key
        unsigned int index = (hash(key) % room);

        // Find the entry
        for (;;)
        {
            Entry *entry = &entries[index];

            // If the key is the same or the entry isn't used, return it
            if (!entry->used)
                return entry;
            else if (compare(entry->key, key))
                return entry;

            // Collision? Increase the index by one
            index = ((index + 1) % room);
        }
    }

    // Adjust entries
    inline void adjust()
    {
        // Increase room
        room <<= 1;

        // Allocate new entries
        Entry *entries = reinterpret_cast<Entry *>(memory_allocate(room * sizeof(Entry)));

        // Fill entries with empty entries
        for (int i = 0; i < room; ++i)
            entries[i].used = false;

        // Add old data to entries
        for (int i = 0; i < size; ++i)
        {
            // Get entry
            Entry *entry = &data[i];
            if (!entry->used) continue;

            // Add entry
            Entry *dest        = find_entry(entries, entry->key);
                   dest->key   = entry->key;
                   dest->value = entry->value;
                   dest->used  = true;
        }

        // Free old data and set to entries
        memory_free(data);
        data = entries;
    }

    // Set key value
    inline Value set(const Key &key, const Value &value)
    {
        // Adjust?
        if (size + 1 >= room)
            adjust();

        // Find entry
        Entry *entry = find_entry(data, key);

        // New entry?
        if (!entry->used)
            ++size;

        // Set entry data
        entry->key   = key;
        entry->value = value;
        entry->used  = true;

        return value;
    }

    // Get key value
    inline Value get(const Key &key)
    {
        // Find entry
        Entry *entry = find_entry(data, key);

        return entry->value;
    }

    // Key exists?
    inline bool exists(const Key &key)
    {
        // Find entry
        Entry *entry = find_entry(data, key);

        // Check
        if (!entry->used || !compare(entry->key, key))
            return false;

        return true;
    }

    // Remove key
    inline bool remove(const Key &key)
    {
        // Find entry
        Entry *entry = find_entry(data, key);

        // Check
        if (!entry->used || !compare(entry->key, key))
            return false;

        // Remove
        entry->used = false;
        return true;
    }

    // Clear
    inline void clear()
    {
        // Fill data with empty entries
        for (int i = 0; i < room; ++i)
            data[i].used = false;
    }

    // Free
    inline void free()
    {
        memory_free(data);
    }

    // Compound Operators
    inline Value operator+=(const EntryInitializer &entry)
    {
        return set(entry.key, entry.value);

    }

    inline bool  operator-=(const Key &key)
    {
        return remove(key);
    }

    // Get value by key
    inline Value &operator[](const Key &key)
    {
        // Adjust?
        if (size + 2 >= room)
            adjust();

        return find_entry(data, key)->value;
    }
};

#endif