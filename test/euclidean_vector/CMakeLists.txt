cxx_test(
   TARGET euclidean_vector_test1
   FILENAME "euclidean_vector_test1.cpp"
   LINK euclidean_vector fmt::fmt-header-only
)

cxx_test(
   TARGET euclidean_vector_test
   FILENAME "euclidean_vector_test.cpp"
   LINK euclidean_vector fmt::fmt-header-only
)
