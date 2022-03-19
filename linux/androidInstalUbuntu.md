# Install Ubuntu Gnome Desktop on android 


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Install Ubuntu Gnome Desktop on android](#install-ubuntu-gnome-desktop-on-android)
  - [Link](#link)

<!-- /code_chunk_output -->

1. Install [termux](https://github.com/termux/termux-app/releases)
2. run command on termux :
    - `pkg update `
    - `pkg install proot-distro ` 
    - `proot-distro install ubuntu`
    - `proot-distro login ubuntu`
    - `apt update -y`
    - `apt upgrade`
    - `apt install sudo vim -y`
    - `adduser $USERNAME`
    - `vim /etc/sudoers`
    ```sh
    # User privilege specification 
    root ALL=(ALL:ALL) ALL 
    $USERNAME ALL=(ALL:ALL) ALL # add it 
    ```
    - `su $USERNAME`
    - `sudo apt install ubuntu-gnome-desktop tightvncserver -y`
    - `sudo apt install gnome-panel gnome-settings-daemon metacity nautilus     gnome-terminal`
    - `service dbus start`
    - `vncserver`
    - `vim ~/.vnc/xstartup`
    ```sh
      # Uncomment the following two lines for normal desktop:
      # unset SESSION_MANAGER
      # exec /etc/X11/xinit/xinitrc
      [ -x /etc/vnc/xstartup ] && exec /etc/vnc/xstartup
      [ -r $HOME/.Xresources ] && xrdb $HOME/.Xresources
      xsetroot -solid grey
      vncconfig -iconic &
      x-terminal-emulator -geometry 80x24+10+10 -ls -title "$VNCDESKTOP Desktop" &
      x-window-manager &
      export XKL_XMODMAP_DISABLE=1
      export XDG_CURRENT_DESKTOP="GNOME-Flashback:GNOME"
      export XDG_MENU_PREFIX="gnome-flashback-"
      gnome-session --session=gnome-flashback-metacity --disable-acceleration-check       --debug &
      gnome-panel &
      gnome-settings-daemon &
      metacity &
      nautilus &
    ```
    - stop vnc server :`vncserver -kill:1`
    - start vncserver agin: `vncserver -geometry 2800x1752`






## Link 
- [Install Ubuntu Gnome Desktop on android | No root](https://m.youtube.com/watch?v=sGgbRqmXtU4)
- [How to Install Ubuntu Gnome Desktop on android without root .](https://technical-bot.blogspot.com/2021/10/how-to-install-ubuntu-gnome-desktop-on.html)

---
- [上一级](README.md)
- 下一篇 -> [fedora install boostnote](boostnote.md)
