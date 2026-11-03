# Morales Research Sun operating system (Public Beta) Live ISO builder 
# v6.1

based on helloSystem (credit to them!)

## Release (Old)

The latest release build 10.0 (Beta 5) "this is the old MR UNIX operating system" can be downloaded [here](https://gitlab.com/morales-research-corporation/os-development/mr-unix/-/releases).

# New Release (v6.1-pb)

The new release is now avaliable to download on [Github](https://github.com/moralesresearch/ISO/releases)

## Building

The ISO is built for the FreeBSD release of the machine it is built on, so build on a supported FreeBSD 14 release (14.4 or later):

```
sudo sh build.sh sunos
```

To build the server edition, which adds the packages in `settings/packages.sunos.server`, uses the `server-master` branch of [Utilities](https://github.com/moralesresearch/Utilities) and identifies itself as Sun/OS Server:

```
sudo EDITION=server sh build.sh sunos
```

(`EDITION` must come after `sudo`, which does not pass on environment variables set before it. This needs a sudoers rule that allows setting variables, such as `ALL`; as root, run `EDITION=server sh build.sh sunos` instead.)

The ISO is written to `/usr/local/furybsd/iso/`. Before building, `sh scripts/check-tree.sh` checks the overlay and settings files for consistency; CI also checks the package lists against the FreeBSD package repository.

## System Requirements for live media

* 2 GHz dual core processor
* 4 GiB RAM (system memory for physical and virtualized installs)
* VGA capable of 1024x768 screen resolution 
* Either a CD/DVD drive or a USB port for booting the installer media

## Credentials for live media

There is no password for `liveuser`. The `liveuser` account is removed upon install.  There is also no root password until it is set in the installer. You can become root using `sudo -i`.

## Acknowledgements

Please see https://hellosystem.github.io/docs/developer/acknowledgements.
