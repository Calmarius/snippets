/* Generates a pseudorandom number from the previous one. */

static unsigned getRand(unsigned *state)
{
    *state = (1103515245* *state + 12345) % 0x7FFFFFFF;
    return *state;
}


#ifdef UNIT_TEST
int main()
{
    (void)getRand;
    return 0;
}

#endif
