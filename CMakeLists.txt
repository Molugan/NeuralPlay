cmake_minimum_required (VERSION 3.0.2)
project( NeuralPlay )


#### Step one : set the directory where libraries should be built in
set( LIB_DIR "${PROJECT_BINARY_DIR}/lib")
file( MAKE_DIRECTORY ${LIB_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_DIR})

INCLUDE_DIRECTORIES("include")

#Data Reader
file(GLOB DATA_READER_SRC "src/DataReader/*.cpp" )
file(GLOB DATA_READER_INCLUDE "include/DataReader/*.h")

set(DATA_READER_LIB "DataReaderLib")
add_library(
    ${DATA_READER_LIB}
    SHARED
    ${DATA_READER_SRC}
    ${DATA_READER_INCLUDE}
)

#Othello
file(GLOB OTHELLO_BASE_SRC "src/Othello/*.cpp")
file(GLOB OTHELLO_BASE_INCLUDE "include/Othello/*.h")

set(OTHELLO_BASE_LIB "OthelloLib")
add_library(
    ${OTHELLO_BASE_LIB}
    SHARED
    ${OTHELLO_BASE_SRC}
    ${OTHELLO_BASE_INCLUDE}
)

#Neural Network
file(GLOB NEURAL_NETWORK_SRC "src/NeuralNetwork/*.cpp")
file(GLOB NEURAL_NETWORK_INCLUDE "include/NeuralNetwork/*.h")

set(NEURAL_NETWORK_LIB "NeuralNetworkLib")
add_library(
    ${NEURAL_NETWORK_LIB}
    SHARED
    ${NEURAL_NETWORK_SRC}
    ${NEURAL_NETWORK_INCLUDE}
)

target_link_libraries(${NEURAL_NETWORK_LIB} ${OTHELLO_BASE_LIB})

#Players
file(GLOB PLAYER_SRC "src/Player/*.cpp")
file(GLOB PLAYER_INCLUDE "include/Player/*.h")

set(PLAYER_LIB "PlayerLib")
add_library(
    ${PLAYER_LIB}
    SHARED
    ${PLAYER_SRC}
    ${PLAYER_INCLUDE}
)

target_link_libraries(${PLAYER_LIB} ${NEURAL_NETWORK_LIB} ${OTHELLO_BASE_LIB})

############################
#Executables

add_executable(IrisClassifier src/IrisClassifier)
target_link_libraries(IrisClassifier ${NEURAL_NETWORK_LIB} ${DATA_READER_LIB})

add_executable(OthelloMatch src/OthelloMatch)
target_link_libraries(OthelloMatch ${NEURAL_NETWORK_LIB} ${PLAYER_LIB} ${OTHELLO_BASE_LIB})

#Unit Tests
add_executable(UTIrisDataLoader src/UTs/UTIrisDataLoader.cpp)
target_link_libraries(UTIrisDataLoader ${DATA_READER_LIB})

add_executable(UTNeuralNet src/UTs/UTNeuralNet)
target_link_libraries(UTNeuralNet ${NEURAL_NETWORK_LIB})