/**
 INTEL CONFIDENTIAL

 Copyright (C) 2007 by Intel Corporation.
 All Rights Reserved.

 The source code contained or described herein and all documents
 related to the source code ("Material") are owned by Intel Corporation
 or its suppliers or licensors. Title to the Material remains with Intel
 Corporation or its suppliers and licensors. The Material contains trade
 secrets and proprietary and confidential information of Intel or its
 suppliers and licensors. The Material is protected by worldwide copyright
 and trade secret laws and treaty provisions. No part of the Material may
 be used, copied, reproduced, modified, published, uploaded, posted,
 transmitted, distributed, or disclosed in any way without Intel's prior
 express written permission.

 No License under any patent, copyright, trade secret or other intellectual
 property right is granted to or conferred upon you by disclosure or
 delivery of the Materials, either expressly, by implication, inducement,
 estoppel or otherwise. Any license under such intellectual property rights
 must be express and approved by Intel in writing.

*/

#include <fstream>
#include "common.h"
#include "check.h"
#include "omp.h"


class BATest: public :: testing::Test
{
protected:
  size_t num_bandwidth;
  int *bandwidth;

  void SetUp()
  {
    const char *node_bandwidth_path = "/etc/numakind/node-bandwidth";
    std::ifstream nbw_file;

    nbw_file.open(node_bandwidth_path, std::ifstream::binary);
    nbw_file.seekg(0, nbw_file.end);
    num_bandwidth = nbw_file.tellg()/sizeof(int);
    nbw_file.seekg(0, nbw_file.beg);
    bandwidth = new int[num_bandwidth];
    nbw_file.read((char *)bandwidth, num_bandwidth*sizeof(int));
    nbw_file.close();
  }

  void TearDown()
  {
    delete[] bandwidth;
  }

};


TEST_F(BATest, HBW_IsHBWAvailable) {
  ASSERT_EQ(1, HBW_IsHBWAvailable());
}

TEST_F(BATest, HBW_policy) {
  EXPECT_EQ(1, HBW_getpolicy());
  HBW_setpolicy(2);
  EXPECT_EQ(2, HBW_getpolicy());
}

TEST_F(BATest, HBW_malloc_2B) {
  size_t size = (size_t)(2);
  char *ptr;
  ASSERT_TRUE((ptr = (char *)HBW_malloc(size)) != NULL);
  memset(ptr, 0, size);
  Check check(ptr, size);
  EXPECT_EQ(0, check.check_node_hbw(num_bandwidth, bandwidth));
  HBW_free(ptr);
}

TEST_F(BATest, HBW_malloc_2KB) {
  size_t size = (size_t)(2*KB);
  char *ptr;
  ASSERT_TRUE((ptr = (char *)HBW_malloc(size)) != NULL);
  memset(ptr, 0, size);
  Check check(ptr, size);
  EXPECT_EQ(0, check.check_node_hbw(num_bandwidth, bandwidth));
  HBW_free(ptr);
}

TEST_F(BATest, HBW_malloc_2MB) {
  size_t size = (size_t)(2*MB);
  char *ptr;
  ASSERT_TRUE((ptr = (char *)HBW_malloc(size)) != NULL);
  memset(ptr, 0, size);
  Check check(ptr, size);
  EXPECT_EQ(0, check.check_node_hbw(num_bandwidth, bandwidth));
  HBW_free(ptr);
}

TEST_F(BATest, HBW_malloc_2GB) {
  size_t size = (size_t)(2*GB);
  char *ptr;
  ASSERT_TRUE((ptr = (char *)HBW_malloc(size)) != NULL);
  memset(ptr, 0, size);
  Check check(ptr, size);
  EXPECT_EQ(0, check.check_node_hbw(num_bandwidth, bandwidth));
  HBW_free(ptr);
}

TEST_F(BATest, HBW_calloc_2B) {
  size_t size = (size_t)(2);
  char *ptr;
  ASSERT_TRUE((ptr = (char *)HBW_calloc(size, 1)) != NULL);
  Check check(ptr, size);
  EXPECT_EQ(0, check.check_zero());
  memset(ptr, 0, size);
  EXPECT_EQ(0, check.check_node_hbw(num_bandwidth, bandwidth));
  HBW_free(ptr);
}

TEST_F(BATest, HBW_calloc_2KB) {
  size_t size = (size_t)(2*KB);
  char *ptr;
  ASSERT_TRUE((ptr = (char *)HBW_calloc(size, 1)) != NULL);
  Check check(ptr, size);
  EXPECT_EQ(0, check.check_zero());
  memset(ptr, 0, size);
  EXPECT_EQ(0, check.check_node_hbw(num_bandwidth, bandwidth));
  HBW_free(ptr);
}

TEST_F(BATest, HBW_calloc_2MB) {
  size_t size = (size_t)(2*MB);
  char *ptr;
  ASSERT_TRUE((ptr = (char *)HBW_calloc(size, 1)) != NULL);
  Check check(ptr, size);
  EXPECT_EQ(0, check.check_zero());
  memset(ptr, 0, size);
  EXPECT_EQ(0, check.check_node_hbw(num_bandwidth, bandwidth));
  HBW_free(ptr);
}

TEST_F(BATest, HBW_calloc_2GB) {
  size_t size = (size_t)(2*GB);
  char *ptr;
  ASSERT_TRUE((ptr = (char *)HBW_calloc(size, 1)) != NULL);
  Check check(ptr, size);
  EXPECT_EQ(0, check.check_zero());
  memset(ptr, 0, size);
  EXPECT_EQ(0, check.check_node_hbw(num_bandwidth, bandwidth));
  HBW_free(ptr);
}

TEST_F(BATest, HBW_realloc_2KB_2MB_2KB) {
  size_t size0 = (size_t)(2*KB);
  size_t size1 = (size_t)(2*MB);
  char *ptr;
  ASSERT_TRUE((ptr = (char *)HBW_realloc(NULL, size0)) != NULL);
  memset(ptr, 0, size0);
  Check check1(ptr, size0);
  EXPECT_EQ(0, check1.check_node_hbw(num_bandwidth, bandwidth));
  ASSERT_TRUE((ptr = (char *)HBW_realloc(ptr, size1)) != NULL);
  memset(ptr, 0, size1);
  Check check2(ptr, size1);
  EXPECT_EQ(0, check2.check_node_hbw(num_bandwidth, bandwidth));
  ASSERT_TRUE((ptr = (char *)HBW_realloc(ptr, size0)) != NULL);
  memset(ptr, 0, size0);
  Check check3(ptr, size0);
  EXPECT_EQ(0, check3.check_node_hbw(num_bandwidth, bandwidth));
  HBW_free(ptr);
}

TEST_F(BATest, HBW_allocate_memalign_2B) {
  void *ptr = NULL;
  size_t size = (size_t)(2);
  size_t align = 32;
  ASSERT_EQ(0, HBW_allocate_memalign(&ptr, align, size));
  ASSERT_TRUE(ptr != NULL);
  EXPECT_EQ(0, (size_t)ptr % align);
  Check check(ptr, size);
  EXPECT_EQ(0, check.check_node_hbw(num_bandwidth, bandwidth));
  HBW_free(ptr);
}

TEST_F(BATest, HBW_allocate_memalign_2KB) {
  void *ptr = NULL;
  size_t size = (size_t)(2*KB);
  size_t align = 32;

  ASSERT_EQ(0, HBW_allocate_memalign(&ptr, align, size));
  ASSERT_TRUE(ptr != NULL);
  EXPECT_EQ(0, (size_t)ptr % align);
  Check check(ptr, size);
  EXPECT_EQ(0, check.check_node_hbw(num_bandwidth, bandwidth));
  HBW_free(ptr);
}

TEST_F(BATest, HBW_allocate_memalign_2MB) {
  void *ptr = NULL;
  size_t size = (size_t)(2*MB);
  size_t align = 32;
  ASSERT_EQ(0, HBW_allocate_memalign(&ptr, align, size));
  ASSERT_TRUE(ptr != NULL);
  EXPECT_EQ(0, (size_t)ptr % align);
  Check check(ptr, size);
  EXPECT_EQ(0, check.check_node_hbw(num_bandwidth, bandwidth));
  HBW_free(ptr);
}

TEST_F(BATest, HBW_allocate_memalign_2GB) {
  void *ptr = NULL;
  size_t size = (size_t)(2*GB);
  size_t align = 32;
  ASSERT_EQ(0, HBW_allocate_memalign(&ptr, align, size));
  ASSERT_TRUE(ptr != NULL);
  EXPECT_EQ(0, (size_t)ptr % align);
  Check check(ptr, size);
  EXPECT_EQ(0, check.check_node_hbw(num_bandwidth, bandwidth));
  HBW_free(ptr);
}

TEST_F(BATest, HBW_allocate_memalign_psize_2B) {
  void *ptr = NULL;
  size_t size = (size_t)(2);
  hbw_pagesize_t hbw_psize = HBW_PAGESIZE_4KB;
  size_t psize = 4*KB;
  size_t align = 8;
  ASSERT_EQ(0, HBW_allocate_memalign_psize(&ptr, align, size, hbw_psize));
  ASSERT_TRUE(ptr != NULL);
  Check check(ptr, size);
  EXPECT_EQ(0, check.check_node_hbw(num_bandwidth, bandwidth));
  EXPECT_EQ(0, check.check_page_size(psize));
  HBW_free(ptr);
}

TEST_F(BATest, HBW_allocate_memalign_psize_2KB) {
  void *ptr = NULL;
  size_t size = (size_t)(2*KB);
  hbw_pagesize_t hbw_psize = HBW_PAGESIZE_4KB;
  size_t psize = 4*KB;
  size_t align = 128;
  ASSERT_EQ(0, HBW_allocate_memalign_psize(&ptr, align, size, hbw_psize));
  ASSERT_TRUE(ptr != NULL);
  Check check(ptr, size);
  EXPECT_EQ(0, check.check_node_hbw(num_bandwidth, bandwidth));
  EXPECT_EQ(0, check.check_page_size(psize));
  HBW_free(ptr);
}

TEST_F(BATest, HBW_allocate_memalign_psize_2MB) {
  void *ptr = NULL;
  size_t size = (size_t)(2*MB);
  hbw_pagesize_t hbw_psize = HBW_PAGESIZE_2MB;
  size_t psize = 2*MB;
  size_t align = 128;
  ASSERT_EQ(0, HBW_allocate_memalign_psize(&ptr, align, size, hbw_psize));
  ASSERT_TRUE(ptr != NULL);
  Check check(ptr, size);
  EXPECT_EQ(0, check.check_node_hbw(num_bandwidth, bandwidth));
  EXPECT_EQ(0, check.check_page_size(psize));
  HBW_free(ptr);
}

TEST_F(BATest, HBW_allocate_memalign_psize_2GB) {
  void *ptr = NULL;
  size_t size = (size_t)(2*GB);
  hbw_pagesize_t hbw_psize = HBW_PAGESIZE_2MB;
  size_t psize = 2*MB;
  size_t align = 128;
  ASSERT_EQ(0, HBW_allocate_memalign_psize(&ptr, align, size, hbw_psize));
  ASSERT_TRUE(ptr != NULL);
  Check check(ptr, size);
  EXPECT_EQ(0, check.check_node_hbw(num_bandwidth, bandwidth));
  EXPECT_EQ(0, check.check_page_size(psize));
  HBW_free(ptr);
}



