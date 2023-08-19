# alice
An open source monolithic kernel for Playstation Vita's ARM Cortex-A9 MPCore main application processor.<br>
It is intended to be used as a companion to [bob](https://github.com/SKGleba/bob) for research and reverse engineering of the Playstation Vita video game console.

## note
This project is in its very early stages, it also serves as a way for me to learn arm & multi-core programming.<br>
Some code might make no sense and/or be overcomplicated, suggestions and PRs are welcome.

## loading alice
the following steps should be done on the Toshiba MeP security processor.<br>
bob's rpc or code exec features are the preferred method of loading alice.
- put arm in reset via [pervasive](https://wiki.henkaku.xyz/vita/Pervasive#ScePervasiveReset_.280xE3101000.29)
- copy alice to 0x1f000000 or 0x40000000
- call or replicate bob's [compat_armReBoot(int armClk, bool hasCS, bool remap_00)](https://github.com/SKGleba/bob/blob/main/source/compat.c)
  - armClk is the desired [arm clock idx](https://wiki.henkaku.xyz/vita/Pervasive#ARM_Clocks)
  - hasCS (0 / 1) enables ARM CoreSight, only present on Development Kits
  - remap_00 (0 / 1) sets ARM's 0x0 as alias of 0x1f000000 or 0x40000000
- ARM will be reset to alice at 0x0
  - in some rare cases, replacing trustzone with alice will crash arm entirely, just reset it again.
