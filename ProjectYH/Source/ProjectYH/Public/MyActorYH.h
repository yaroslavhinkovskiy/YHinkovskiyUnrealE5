// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#ifndef TARGETFINDER_H
#define TARGETFINDER_H

#include <cmath>
#include <string>

struct Vector3 {
    double x, y, z;

    Vector3 operator-(const Vector3& other) const {
        return { x - other.x, y - other.y, z - other.z };
    }

    double Length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vector3 Normalized() const {
        double len = Length();
        return (len > 0) ? Vector3{ x / len, y / len, z / len } : Vector3{ 0, 0, 0 };
    }
};

struct TargetData {
    Vector3 Direction;
    double Angle;
    std::string Side;
};

#endif // TARGETFINDER_H
