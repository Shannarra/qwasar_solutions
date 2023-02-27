# my_tar


## How to run

```sh
make && ./my_tar option(s) tarfile [sources]
```

For example you can create a new tarball with:
```
make && ./my_tar -cf file.tar tar.c tar.h
```
This will create a file called `file.tar` that will contain the atchived versions for the `tar.c` and `tar.h` files.   
After that to test if this really is a tar file we can run the Linux command:
```
tar -tf file.tar
```

Additionally my_tar can read the tarfile by itself:
```
make && ./my_tar -tf file.tar
```

## Personal

Created by Petar Angelov - Shannarra
IDE used - Emacs