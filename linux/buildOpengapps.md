# Build openGapps from github

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->
<!-- code_chunk_output -->

* [Build openGapps from github](#build-opengapps-from-github)
	* [Install git and lzip](#install-git-and-lzip)
	* [Install android build tools and set $PATH](#install-android-build-tools-and-set-path)
	* [Install JDK and set $PATH](#install-jdk-and-set-path)
	* [Download open Gapps from github](#download-open-gapps-from-github)
	* [Build arm64-8.1-full](#build-arm64-81-full)
	* [Link](#link)

<!-- /code_chunk_output -->


## Install git and lzip

In fedora:
```sh
dnf install git lzip -y
```
## Install android build tools and set $PATH
Download offline build tools

```sh
vim .bashrc
```

```sh
export PATH=$PATH:/home/breap/Android/Sdk/build-tools/27.0.3
```

```sh
source ~/.bashrc
```

## Install JDK and set $PATH
Download JDK

```sh
vim .bashrc
```

```sh
JAVA_HOME=/usr/java/jdk1.8.0_144
PATH=$PATH:$JAVA_HOME/bin
export JAVA_HOME
export PATH           
```

```sh
source ~/.bashrc
```

## Download open Gapps from github

```sh
git clone git@github.com:opengapps/opengapps.git
```

```sh
# Usage:
./download_sources.sh [--shallow] [arch]
```

* ```--shallow``` will order to fetch only the latest snapshot of the APKs (reduces space used and amount of data to be retrieved by git, by not fetching the APKs' history)
* ```arch``` can be one of the following "arm, arm64, x86, x86_64" to fetch only data required for specified architecture (note that fallback architectures will be fetched too)

```sh
cd $opengapps_home
./download_sources.sh --shallow arm64
# will download 20G file  
```

```sh
cd $opengapps_home
./download_sources.sh arm64
# will download 66G file  
```

## Build arm64-8.1-full

```sh
make arm64-27-full
```

## Link

[opengapps github](https://github.com/opengapps/opengapps)
[上一级](README.md)
[上一篇 -> fedora install boostnote](boostnote.md)
[下一篇 -> 理解chroot](chroot.md)
