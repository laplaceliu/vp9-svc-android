## Prerequisite
You need Go environment to get [blueprint](https://github.com/google/blueprint) build system.

```bash
export GOROOT=/your/goroot/path # My GOROOT is /usr/local/Cellar/go/1.15/libexec
export GOPATH=/your/go/path     # My GOPATH is $HOME/dev/gopath
```


make sure your `$GOPATH/bin` is in `$PATH`
```bash
export PATH="$GOPATH/bin:$PATH"
```

```go
go get -u github.com/google/blueprint/...
```

check if you have `bpfmt`
```bash
$ bpfmt
one of -d, -l, -o, or -w is required
usage: bpfmt [flags] [path ...]
  -d	display diffs instead of rewriting files
  -l	list files whose formatting differs from bpfmt's
  -o	write result to stdout
  -s	sort arrays
  -w	write result to (source) file
```


For macOS, you need some GNU-version command line tools

```bash
brew install findutils # xargs
brew install gnu-sed   # sed
```

and then make sure GNU-version command line tools are having high priority than macOS-version

```bash
export PATH="/usr/local/opt/findutils/libexec/gnubin:$PATH"
export PATH="/usr/local/opt/gnu-sed/libexec/gnubin:$PATH"
```

## 
```bash
./update_libvpx.sh [tag/branch]
```

```bash
./generate_config.sh
```

```bash
./libvpx/configure --target=arm64-android-gcc --enable-external-build
```

```bash
ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=./Android.mk \
APP_ABI=arm64-v8a APP_PLATFORM=android-21 APP_OPTIM=release \
APP_STL=c++_static NDK_TOOLCHAIN_VERSION=clang ENABLE_SHARED=0
```


## Generate Android jni files

using [swig](http://www.swig.org/)
```
swig -v -java -package your.package.name -outdir your/output/path -o your/jni/wrapper/vp9_svc_encoder_wrap.c vp9_svc_encoder.i
```

Note that it is common to spicify the `outdir` to `app/src/main/java/your/package/name` if you spicified the `package` to `your.package.name`.
The `xxx_wrap.c` and `xxx.i` are commonly stored in `app/src/main/cpp`.

For example:
```
swig -v -java -package io.byteplanet.encoder -outdir src/main/java/io/byteplanet/encoder -o src/main/cpp/vp9_svc_encoder_wrap.c src/main/cpp/vp9_svc_encoder.i
```
