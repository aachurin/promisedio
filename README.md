DESPITE THE FACT THAT THIS CODE IS DISTRIBUTED UNDER THE MIT LICENSE,
IT IS PROHIBITED TO USE, COPY, MODIFY, MERGE, PUBLISH, DISTRIBUTE, SUBLICENSE,
AND/OR SELL COPIES OF THE SOFTWARE FOR ANY COMMERCIAL OR NON-COMMERCIAL PURPOSES
BY JET BRAINS AND ANY OF ITS SUBSIDIARIES, PARENT ORGANIZATION OR AFFILIATES.

<p align="center">
    <img src="https://raw.githubusercontent.com/aachurin/promisedio/main/logo.svg" alt="PromisedIO" />
</p>
<p align="center">
    <b>PromisedIO</b>
</p>

###### PromisedIO is free and open source software released under the permissive MIT license.

### promisedio.promise (Promises)
You can read about promises [here](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Promise).

```python
deferred() -> Deferred
```
Create new _Deferred_ object.

```python
Deferred.promise() -> Promise
```
Get related _Promise_ object.

```python
Deferred.resolve(value: object) -> None
```
Resolve related _Promise_ object with the given _value_.

```python
Deferred.reject(exc: Exception) -> None
```
Reject related _Promise_ object with the given exception _exc_.

```python
Promise.then(fulfilled: Callable[[object], object], rejected: Callable[[Exception], object]) -> Promise
```
Create new _Promise_.

It takes up to two arguments: callback functions for the success and failure cases of the _Promise_.
 
```python
Promise.catch(rejected: Callable[[Exception], object]) -> Promise
```
Create new _Promise_.

It is the same as `.then(None, rejected)`


```python
exec_async(coro: Coroutine) -> None
```
Start coroutine.

##### Example:
```python
...

def my_sleep(timeout):
    d = promise.deferred()
    timer.set_timeout(lambda: d.resolve("My rules"), timeout)
    return d.promise()


async def main():
    print(await my_sleep(5.))

    
promise.exec_async(main())
loop.run_forever()
```

### promisedio.timer (Timers)

```python
sleep(seconds: float) -> Promise[None]
```
Delay execution for a given number of seconds.

```python
set_timeout(func: Callable[[], None], timeout: float, *, unref: bool = False) -> Timer
```
Set timer which executes a function once the timer expires.

Return _Timer_ object. This value can be passed to _clear_timeout()_ to cancel the timeout.

```python
set_interval(func: Callable[[], None], timeout: float, *, unref: bool = False) -> Timer
```
Set timer which executes repeatedly a function, with a fixed time delay between each call.

Return _Timer_ object. This value can be passed to _clear_interval()_ to cancel the interval.

```python
clear_timeout(ob: Timer) -> None
```
Cancel timeout previously established by calling _set_timeout()_.

```python
clear_interval(ob: Timer) -> None
```
Cancel interval previously established by calling _set_interval()_.

```python
time() -> int
```
Get current timestamp in milliseconds.

The timestamp is cached at the start of the event loop tick.

```python
hrtime() -> int
```
Return current high-resolution real time. 

This is expressed in nanoseconds. 

### promisedio.fs (File system operations)
```python
close(fd: int) -> Promise[None]
```
Close file descriptor _fd_.

Equivalent to [close(2)](https://man7.org/linux/man-pages/man2/close.2.html).

```python
fstat(fd: int) -> Promise[StatObj]
```
Get the status of the file descriptor _fd_. 

Return _StatObj_ object.

Equivalent to [fstat(2)](https://man7.org/linux/man-pages/man2/fstat.2.html).

```python
openfd(path: Union[Path, str, bytes], flags: int, mode: int = 0o666) -> Promise[int]
```
Open the file path and set various flags according to flags and possibly its mode according to mode.

Return the file descriptor for the newly opened file. The new file descriptor is non-inheritable.

Equivalent to [open(2)](https://man7.org/linux/man-pages/man2/open.2.html).

```python
open(path: Union[Path, str, bytes, int], flags: str = "r", closefd: bool = True) -> Promise[FileIO]
```
Open file and return a corresponding file object. If the file cannot be opened, an _OSError_ is raised. 

Equivalent to python [open](https://docs.python.org/3/library/functions.html#open) (binary mode only).

```python
read(fd: int, size: int = -1, offset: int = -1) -> Promise[bytes]
```
Read from file descriptor _fd_ until we have _size_ characters or we hit EOF.

If _offset_ is present and is not negative, read at most _size_ bytes from file descriptor _fd_ at a position of _offset_,
leaving the file offset unchanged.

Equivalent to [read(2)](https://man7.org/linux/man-pages/man2/read.2.html)
              [preadv(2)](https://man7.org/linux/man-pages/man2/preadv.2.html)

```python
write(fd: int, data: bytes, offset: int = -1) -> Promise[int]
```
Write the _data_ to file descriptor _fd_.

If _offset_ is present and is not negative, write the _data_ to file descriptor _fd_ at position of _offset_, 
leaving the file offset unchanged.

Return the number of bytes actually written.

Equivalent to [write(2)](https://man7.org/linux/man-pages/man2/write.2.html)
              [pwritev(2)](https://man7.org/linux/man-pages/man2/pwritev.2.html)

```python
stat(path: Union[Path, str, bytes], *, follow_symlinks: bool = True) -> Promise[StatObj]
```
Get the status of a file. Return a _StatObj_ object.

This function normally follows symlinks; to stat a symlink add the argument _follow_symlinks=False_

Equivalent to [stat(2)](https://man7.org/linux/man-pages/man2/stat.2.html) 
              [lstat(2)](https://man7.org/linux/man-pages/man2/lstat.2.html).

```python
seek(fd: int, pos: int, how: int) -> Promise[int]
```
Set the current position of file descriptor _fd_ to position _pos_, modified by how:
- _SEEK_SET_ or _0_ to set the position relative to the beginning of the file;
- _SEEK_CUR_ or _1_ to set it relative to the current position;
- _SEEK_END_ or _2_ to set it relative to the end of the file.

Return the new cursor position in bytes, starting from the beginning.

Equivalent to [lseek(2)](https://man7.org/linux/man-pages/man2/lseek.2.html).

```python
unlink(path: Union[Path, str, bytes]) -> Promise[None]
```
Remove (delete) the file _path_.

Equivalent to [unlink(2)](https://man7.org/linux/man-pages/man2/unlink.2.html).

```python
mkdir(path: Union[Path, str, bytes], mode: int = 0o777) -> Promise[None]
```
Create a directory named _path_ with numeric _mode_ mode.

If the directory already exists, _FileExistsError_ is raised.

On some systems, _mode_ is ignored.

Equivalent to [mkdir(2)](https://man7.org/linux/man-pages/man2/mkdir.2.html).

```python
rmdir(path: Union[Path, str, bytes]) -> Promise[None]
```
Remove (delete) the directory path. 

If the directory does not exist or is not empty, an _FileNotFoundError_ or an _OSError_ is raised respectively.

Equivalent to [rmdir(2)](https://man7.org/linux/man-pages/man2/rmdir.2.html).

```python
mkdtemp(tpl: Union[Path, str, bytes]) -> Promise[bytes]
```
Generate a uniquely named temporary directory from template _tpl_.

The last six characters of template must be XXXXXX and these are replaced with a string that makes
the directory name unique.

Return the modified template string.

Equivalent to [mkdtemp(3)](https://man7.org/linux/man-pages/man3/mkdtemp.3.html).

```python
mkstemp(tpl: Union[Path, str, bytes]) -> Promise[Tuple[int, bytes]]
```
Generate a unique temporary filename from template.

This function creates and opens the file, and returns an open file descriptor for the file.
The last six characters of template must be "XXXXXX" and these are replaced with a string that makes 
the filename unique.

Return the file descriptor and modified template string.

Equivalent to [mkstemp(3)](https://man7.org/linux/man-pages/man3/mkstemp.3.html). 

```python
scandir(path: Union[Path, str, bytes]) -> Promise[Tuple[Tuple[int, bytes], ...]]
```
Return a sequence of the entries in the directory given by _path_ (entry_type, entry_name).  

Special entries '.' and '..' are not included.

Equivalent to [scandir(3)](https://man7.org/linux/man-pages/man3/scandir.3.html).

```python
rename(path: Union[Path, str, bytes], new_path: Union[Path, str, bytes]) -> Promise[None]
```
Rename the file or directory _path_ to _new_path_.

Equivalent to [rename(2)](https://man7.org/linux/man-pages/man2/rename.2.html).

```python
fsync(fd: int) -> Promise[None]
```
Force write of file with file descriptor _fd_ to disk.

Equivalent to [fsync(2)](https://man7.org/linux/man-pages/man2/fsync.2.html).

```python
fdatasync(fd: int) -> Promise[None]
```
Force write of file with file descriptor _fd_ to disk. Does not force update of metadata.

Equivalent to [fdatasync(2)](https://man7.org/linux/man-pages/man2/fdatasync.2.html).

```python
ftruncate(fd: int, length: int) -> Promise[None]
```
Truncate the file corresponding to file descriptor fd, so that it is at most _length_ bytes in size.

Equivalent to [ftruncate(2)](https://man7.org/linux/man-pages/man2/ftruncate.2.html).

```python
copyfile(path: Union[Path, str, bytes], new_path: Union[Path, str, bytes], flags: int = 0) -> Promise[None]
```
Copy a file from path to new_path. 

Supported flags are: 
- _COPYFILE_EXCL_
- _COPYFILE_FICLONE_
- _COPYFILE_FICLONE_FORCE_

For more information, see [uv_fs_copyfile](http://docs.libuv.org/en/v1.x/fs.html#c.uv_fs_copyfile).

```python
sendfile(out_fd: int, in_fd: int, in_offset: int, count: int) -> Promise[int]
```
Copy _count_ bytes from file descriptor _in_fd_ to file descriptor _out_fd_ starting at _offset_.

Return the number of bytes sent.

Equivalent to [sendfile(2)](https://man7.org/linux/man-pages/man2/sendfile.2.html).

```python
access() -> Promise[bool]
```
Use the real uid/gid to test for access to path.

Mode should be _F_OK_ to test the existence of path, or it can be the inclusive OR of one or more of 
_R_OK_, _W_OK_, and _X_OK_ to test permissions. 

Return _True_ if access is allowed, _False_ if not.

Equivalent to [access(2)](https://man7.org/linux/man-pages/man2/access.2.html).

```python
chmod(path: Union[Path, str, bytes], mode: int) -> Promise[None]
```
Change the mode of _path_ to the numeric _mode_.

See [stat module](https://docs.python.org/3/library/stat.html#stat.S_ISUID) for available _mode_.

Equivalent to [chmod(2)](https://man7.org/linux/man-pages/man2/chmod.2.html).

```python
fchmod(fd: int, mode: int) -> Promise[None]
```
Change the mode of the file given by _fd_ to the numeric _mode_.

See [stat module](https://docs.python.org/3/library/stat.html#stat.S_ISUID) for available _mode_.

Equivalent to [fchmod(2)](https://man7.org/linux/man-pages/man2/fchmod.2.html).

```python
utime(path: Union[Path, str, bytes], atime: float, mtime: float, *, follow_symlinks: bool = True) -> Promise[None]
```
Set the access and modified times of the file specified by _path_.

This function normally follows symlinks.

Equivalent to [utime(2)](https://man7.org/linux/man-pages/man2/utime.2.html)
              [lutimes(2)](https://man7.org/linux/man-pages/man3/lutimes.3.html).

```python 
futime(fd: int, atime: float, mtime: float) -> Promise[None]
```
Set the access and modified times of the file given by _fd_.

Equivalent to [futimes(3)](https://man7.org/linux/man-pages/man3/futimes.3.html)

```python 
link(path: Union[Path, str, bytes], new_path: Union[Path, str, bytes]) -> Promise[None]
```
Create a hard link pointing to _path_ named _new_path_.

Equivalent to [link(2)](https://man7.org/linux/man-pages/man2/link.2.html)

```python 
symlink(path: Union[Path, str, bytes], new_path: Union[Path, str, bytes], flags: int = 0) -> Promise[None]
```
Create a symbolic link pointing to _path_ named _new_path_.

On Windows the _flags_ parameter can be specified to control how the symlink will be created:
 - SYMLINK_DIR: indicates that path points to a directory.
 - SYMLINK_JUNCTION: request that the symlink is created using junction points.

Equivalent to [symlink(2)](https://man7.org/linux/man-pages/man2/symlink.2.html)

```python 
readlink(path: Union[Path, str, bytes]) -> Promise[bytes]
```
Return a string representing the path to which the symbolic link points.

Equivalent to [readlink(2)](https://man7.org/linux/man-pages/man2/readlink.2.html)

```python 
chown(path: Union[Path, str, bytes], uid: int, gid: int, *, follow_symlinks: bool = True) -> Promise[None]
```
Change the owner and group id of path to the numeric uid and gid.

Equivalent to [chown(2)](https://man7.org/linux/man-pages/man2/chown.2.html)

```python 
fchown(fd: int, uid: int, gid: int, *, follow_symlinks: bool = True) -> Promise[None]
```
Change the owner and group id of _fd_ to the numeric uid and gid.

Equivalent to [fchown(2)](https://man7.org/linux/man-pages/man2/fchown.2.html)

### promisedio.ns (Networking)
```python
getaddrinfo(host: Union[str, bytes], port: Union[str, bytes, int], family: int = 0, type: int = 0, proto: int = 0, flags: int = 0) -> Promise
```
Translate the host/port argument into a sequence of 5-tuples that contain all the necessary arguments for 
creating a socket connected to that service. _host_ is a domain name, a string representation of an IPv4/v6 address or _None_.
_port_ is a string service name such as 'http', a numeric port number or _None_. 

For more information, see [getaddrinfo](https://docs.python.org/3/library/socket.html#socket.getaddrinfo)

```python
getnameinfo(sockaddr: Tuple, flags: int) -> Promise[Tuple[str, str]]
```
Translate a socket address sockaddr into a 2-tuple (host, port). 

Depending on the settings of flags, the result can contain a fully-qualified domain name or 
numeric address representation in host.
Similarly, port can contain a string port name or a numeric port number.

For more information about flags you can consult [getnameinfo(3)](https://man7.org/linux/man-pages/man3/getnameinfo.3.html)

```python
open_connection(remote_addr: Tuple, *, local_addr: Tuple = None, limit: int = -1, chunk_size: int = -1, 
                nodelay: int = -1, keepalive: int = -1,         
                ssl: _ssl._SSLContext = None, server_hostname: str = None) -> Promise[TcpStream]:
```
Establish an IPv4 or IPv6 TCP connection.

_limit_: The internal buffer size limit used by the _Stream_ instance. By default, the limit is set to 64 KiB.
_chunk_size_: The minimum size of one chunk of the internal buffer. By default, is set to 496 bytes (on x64).
_nodelay_: Enable/disable TCP_NODELAY. Set 1 to disable Nagle's algorithm.
_keepalive_: Enable/disable TCP keep-alive. The value is the initial delay in seconds. Set 0 to disable TCP keep-alive.

```python
TcpStream.set_tcp_nodelay
```
Enable/disable TCP_NODELAY, which disables Nagle's algorithm.

```python
TcpStream.set_tcp_keepalive
```
Enable/disable TCP keep-alive. Set zero to disable.

```python
TcpStream.getsockname() -> Tuple
```
Return the socket's own address (For TCP only).

```python
TcpStream.getpeername() -> Tuple
```
Get the current address to which the handle is bound (For TCP only)

```python
TcpStream.write(data: bytes) -> Promise[None]
```
Write data to the stream.

```python
TcpStream.read(n: int = -1) -> Promise[bytes]
```
Read up to n bytes. 

If n is not provided, or set to -1, reads as much bytes as possible.
If EOF was received and the internal buffer is empty, return an empty bytes object.

```python
TcpStream.readexactly(n: int) -> Promise[bytes]
```
Read exactly n bytes.

Raise an IncompleteReadError if EOF is reached before n can be read. 
Use the IncompleteReadError.partial attribute to get the partially read data.

Returned value is not limited by the configured stream limit.

```python
TcpStream.readuntil(c: bytes) -> Promise[bytes]
```
Read data from the stream until c is found.

Raise an IncompleteReadError if EOF is reached before n can be found.
Use the IncompleteReadError.partial attribute to get the partially read data.

If the amount of data read exceeds the configured stream limit, a LimitOverrunError exception is raised, 
and the data is left in the internal buffer and can be read again.

```python
TcpStream.shutdown() -> Promise[None]
```
Shut down the stream.

```python
TcpStream.close() -> Promise[None]
```
Close the stream handle.

### promisedio.loop (Loop)

```python
run_forever() -> None
```
Run loop.
