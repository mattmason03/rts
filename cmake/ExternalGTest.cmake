# Add gtest
ExternalProject_Add(
    gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.7.0
    TIMEOUT 30
    CMAKE_ARGS -Dgtest_force_shared_crt=ON
    # Disable install step
    INSTALL_COMMAND ""
    # Wrap download, configure and build steps in a script to log output
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON)

# Specify include dir
ExternalProject_Get_Property(gtest source_dir)
set(gtest_include ${source_dir}/include)

ExternalProject_Get_Property(gtest binary_dir)
set(pref ${binary_dir}/${CMAKE_CFG_INTDIR})
set(gtest_libs ${pref}/gtest.lib ${pref}/gtest_main.lib)
