#pragma once

struct Matrix4 {
    float m[16];

    Matrix4() {
        // Identity matrix
        for (int i = 0; i < 16; ++i) m[i] = 0.0f;
        m[0] = 1.0f; m[5] = 1.0f; m[10] = 1.0f; m[15] = 1.0f;
    }
};