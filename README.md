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
```
./update_libvpx.sh [tag/branch]
```



```
./generate_config.sh
```

```

```