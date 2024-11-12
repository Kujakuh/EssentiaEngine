#include <iostream>
#include <glm/glm.hpp>

// Función para imprimir vectores glm
template <typename T, glm::qualifier Q>
void printVector(const glm::vec<2, T, Q>& vec)
{
    std::cout << "[" << vec.x << ", " << vec.y << "]" << std::endl;
}

template <typename T, glm::qualifier Q>
void printVector(const glm::vec<3, T, Q>& vec)
{
    std::cout << "[" << vec.x << ", " << vec.y << ", " << vec.z << "]" << std::endl;
}

template <typename T, glm::qualifier Q>
void printVector(const glm::vec<4, T, Q>& vec)
{
    std::cout << "[" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << "]" << std::endl;
}

// Función para imprimir matrices glm
template <typename T, glm::qualifier Q>
void printMatrix(const glm::mat<2, 2, T, Q>& mat)
{
    for (int i = 0; i < 2; ++i)
        std::cout << "[" << mat[i][0] << ", " << mat[i][1] << "]" << std::endl;
}

template <typename T, glm::qualifier Q>
void printMatrix(const glm::mat<3, 3, T, Q>& mat)
{
    for (int i = 0; i < 3; ++i)
        std::cout << "[" << mat[i][0] << ", " << mat[i][1] << ", " << mat[i][2] << "]" << std::endl;
}

template <typename T, glm::qualifier Q>
void printMatrix(const glm::mat<4, 4, T, Q>& mat)
{
    for (int i = 0; i < 4; ++i)
        std::cout << "[" << mat[i][0] << ", " << mat[i][1] << ", " << mat[i][2] << ", " << mat[i][3] << "]" << std::endl;
}
