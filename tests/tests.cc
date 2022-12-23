//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//                  MP : Seam Carver  : Student Test Cases                  //
//                                                                          //
// Written By :                         Environment :                       //
// Date ......:                         Compiler ...:                       //
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//                             Framework Set-up                             //
//////////////////////////////////////////////////////////////////////////////
#include "catch.hpp"

// #ifndef CATCH_CONFIG_MAIN
// #  define CATCH_CONFIG_MAIN
// #endif
// #include <catch/catch.hpp>

//////////////////////////////////////////////////////////////////////////////
//                                 Includes                                 //
//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <stdexcept>

#include "image_ppm.hpp"
#include "pixel.hpp"
#include "seam_carver.hpp"

//////////////////////////////////////////////////////////////////////////////
//                                Test Cases                                //
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////

TEST_CASE("True == True", "") { REQUIRE(true == true); }

TEST_CASE("Energy") {
  ImagePPM image("/home/vagrant/src/mp-seam-carver-ninaat2/images/image1.ppm");
  SeamCarver seam(image);

  REQUIRE(seam.SeamCarver::GetEnergy(1, 1) == 0);
  REQUIRE(seam.SeamCarver::GetEnergy(1, 0) == 0);
  REQUIRE(seam.SeamCarver::GetEnergy(1, 3) == 0);
  REQUIRE(seam.SeamCarver::GetEnergy(0, 1) == 525);
  REQUIRE(seam.SeamCarver::GetEnergy(2, 2) == 525);
  REQUIRE(seam.SeamCarver::GetEnergy(0, 0) == 75);
  REQUIRE(seam.SeamCarver::GetEnergy(0, 3) == 450);
  REQUIRE(seam.SeamCarver::GetEnergy(2, 0) == 75);
  REQUIRE(seam.SeamCarver::GetEnergy(2, 3) == 450);

  int* vertical = seam.SeamCarver::GetVerticalSeam();
  REQUIRE(vertical[0] == 0);
  REQUIRE(vertical[1] == 0);
  REQUIRE(vertical[2] == 0);

  int* horizontal = seam.SeamCarver::GetHorizontalSeam();
  REQUIRE(horizontal[0] == 1);
  REQUIRE(horizontal[1] == 1);
  REQUIRE(horizontal[2] == 1);
  REQUIRE(horizontal[3] == 1);

  delete vertical;
  vertical = nullptr;
  delete horizontal;
  horizontal = nullptr;

   seam.RemoveHorizontalSeam();
    REQUIRE(seam.GetImage().GetPixel(1, 1).GetRed() == 5);
    REQUIRE(seam.GetImage().GetPixel(1, 1).GetGreen() == 5);
    REQUIRE(seam.GetImage().GetPixel(1, 1).GetBlue() == 5);
    REQUIRE(seam.GetImage().GetPixel(1, 2).GetGreen() == 0);
    REQUIRE(seam.GetImage().GetPixel(1, 2).GetBlue() == 15);
    // REQUIRE(seam.GetImage().GetPixel(2, 0).GetBlue() == 15); -> should cause seg fault

}

TEST_CASE("Energy their example") {
    ImagePPM
    image("/home/vagrant/src/mp-seam-carver-ninaat2/images/their-example.ppm");
    SeamCarver seam(image);
    REQUIRE(seam.SeamCarver::GetEnergy(1, 1) == 4);
    REQUIRE(seam.SeamCarver::GetEnergy(1, 2) == 4);
    REQUIRE(seam.SeamCarver::GetEnergy(0, 0) == 4);
    REQUIRE(seam.SeamCarver::GetEnergy(2, 2) == 2);
    REQUIRE(seam.SeamCarver::GetEnergy(0, 1) == 3);
    REQUIRE(seam.SeamCarver::GetEnergy(0, 2) == 5);
    REQUIRE(seam.SeamCarver::GetEnergy(1, 0) == 5);
    REQUIRE(seam.SeamCarver::GetEnergy(1, 1) == 4);
    REQUIRE(seam.SeamCarver::GetEnergy(2, 0) == 3);
    REQUIRE(seam.SeamCarver::GetEnergy(2, 1) == 2);

    int* horizontal = seam.SeamCarver::GetHorizontalSeam();
    REQUIRE(horizontal[0] == 2);
    REQUIRE(horizontal[1] == 2);
    REQUIRE(horizontal[2] == 2);
    delete horizontal;
    horizontal = nullptr;
   
   seam.RemoveHorizontalSeam();
    // REQUIRE(seam.GetImage().GetPixel(2, 0).GetBlue() == 15); -> should cause seg fault

}

TEST_CASE("horizontal last col") {
    ImagePPM
    image("/home/vagrant/src/mp-seam-carver-ninaat2/images/horizontal-last-col.ppm");
    SeamCarver seam(image);
    int* horizontal = seam.SeamCarver::GetHorizontalSeam();
    REQUIRE(horizontal[0] == 1);
    REQUIRE(horizontal[1] == 0);
    REQUIRE(horizontal[2] == 1);
    REQUIRE(horizontal[3] == 0);
    delete horizontal;
    horizontal = nullptr;
}
