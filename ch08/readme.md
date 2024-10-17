# 8-1

When we execute the following code, we find that it displays the same number twice, even though the two users have different IDs in the password file. Why is this?

```c
printf("%ld %ld\n", (long) (getpwnam("avr")->pw_uid), (long) (getpwnam("tsr")->pw_uid));
```

## answer

Because `getpwnam` returns its result in a statically allocated buffer. The second call overwrites the result of the first call.

# 8-2

Implement getpwnam() using setpwent(), getpwent(), and endpwent().

## answer

see code in `getpwnam.c`
