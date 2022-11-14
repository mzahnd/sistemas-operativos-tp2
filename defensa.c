struct SHMEM_STRUCT
{
    char  name[MAX_NAME];

    void * mem;
    size_t size;
    int opens;
}

static struct SHMEM_STRUCT shblocks[MAX_BLOCKS] = { };
static int n_blocks = 0;

void* open(char * name, size_t size)
{
    int found = find_block(name);

    if (found != -1)
    {
        shblocks[found].open++;
        return shblocks[found].mem;
    }
    else
    {
        struct SHMEM_STRUCT * ptr = create_block(name, size);
        if (ptr == NULL)
            return NULL;

        return ptr->mem;
    }
}

struct SHMEM_STRUCT * create_block(char * name, size_t size)
{
    void * ptr = malloc(size);
    if (ptr == NULL)
    {
        return NULL;
    }

    shblocks[n_blocks].name = name;
    shblocks[n_blocks].mem = ptr;
    shblocks[n_blocks].size = size;
    shblocks[n_blocks].opens = 1;
    n_blocks++;

    return &shblocks[n_blocks - 1];
}
