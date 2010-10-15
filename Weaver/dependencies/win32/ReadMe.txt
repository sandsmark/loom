Here should be two symbolic links:

Ogre      ->  Link to Ogre SDK install dir
Qt        ->  Link to Qt SDK install dir
replicode ->  Link to replicode source

Creating the links:

mklink /D Ogre <Path to OgreSDK_vc9_v1-7-1 folder>
mklink /D Qt <Path to C:\Qt\4.6.3\ if you are using the default install dir>

Also, it's possible that some additional boost libs have to be downloaded and copied inside Ogre/boost_1_42/lib
