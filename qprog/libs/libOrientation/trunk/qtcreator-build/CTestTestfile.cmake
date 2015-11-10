# CMake generated Testfile for 
# Source directory: /home/atty/Prog/libOrientation/trunk
# Build directory: /home/atty/Prog/libOrientation/trunk/qtcreator-build
# 
# This file includes the relevent testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
ADD_TEST(io_NonValidFiles "/home/atty/Prog/libOrientation/trunk/qtcreator-build/bin/TestOrientationMATIS_NonValidFile" "/home/atty/Prog/libOrientation/trunk/data/RoadSigns.xml")
ADD_TEST(io "/home/atty/Prog/libOrientation/trunk/qtcreator-build/bin/TestOrientationMATIS_IO" "/home/atty/Prog/libOrientation/trunk/data/olivier.xml" "/home/atty/Prog/libOrientation/trunk/data/Sample_conic.xml" "/home/atty/Prog/libOrientation/trunk/data/Amiens296.ori" "/home/atty/Prog/libOrientation/trunk/data/Amiens296.xml" "/home/atty/Prog/libOrientation/trunk/data/Sample_WGS84.xml" "/home/atty/Prog/libOrientation/trunk/data/Amiens296_crope.xml" "/home/atty/Prog/libOrientation/trunk/data/Mairie-090708_0645-22-00001_0000704_1.xml" "/home/atty/Prog/libOrientation/trunk/data/Mairie-090708_0645-34-00001_0000706.ori.xml" "/home/atty/Prog/libOrientation/trunk/data/ORTHO.HDR")
ADD_TEST(samples "/home/atty/Prog/libOrientation/trunk/qtcreator-build/bin/TestOrientationMATIS_samples" "/home/atty/Prog/libOrientation/trunk/data/Sample_WGS84.xml" "/home/atty/Prog/libOrientation/trunk/data/olivier_Projective.xml" "/home/atty/Prog/libOrientation/trunk/data/Amiens296.xml" "/home/atty/Prog/libOrientation/trunk/data/orid.xml" "/home/atty/Prog/libOrientation/trunk/data/ori_test_projective.xml" "/home/atty/Prog/libOrientation/trunk/data/Pano_SampleFC.xml")
ADD_TEST(WorldToImageOldOri "/home/atty/Prog/libOrientation/trunk/qtcreator-build/bin/TestOrientationMATIS_Projection" "/home/atty/Prog/libOrientation/trunk/data/Amiens296.ori")
ADD_TEST(Transfo2DOldOri "/home/atty/Prog/libOrientation/trunk/qtcreator-build/bin/TestOrientationMATIS_Projection" "/home/atty/Prog/libOrientation/trunk/data/orid.ori")
ADD_TEST(PairOrientation "/home/atty/Prog/libOrientation/trunk/qtcreator-build/bin/Test_PairOrientation" "/home/atty/Prog/libOrientation/trunk/data/st_antoine.txt" "/home/atty/Prog/libOrientation/trunk/data/orig_st_antoine.ori" "/home/atty/Prog/libOrientation/trunk/data/orid_st_antoine.ori")
ADD_TEST(Test_OldOriWriting "/home/atty/Prog/libOrientation/trunk/qtcreator-build/bin/Test_OldOriWriting" "/home/atty/Prog/libOrientation/trunk/data/Amiens296.ori" "/home/atty/Prog/libOrientation/trunk/data/orid.ori")
