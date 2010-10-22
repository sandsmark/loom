Here should be two symbolic links:

Ogre      ->  Link to Ogre SDK install dir

Creating the links:

mklink /D Ogre <Path to OgreSDK_vc9_v1-7-1 folder>

Also, it's possible that some additional boost libs have to be downloaded and copied inside Ogre/boost_1_42/lib
