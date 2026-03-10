// Fill out your copyright notice in the Description page of Project Settings.

#include "MyActorYH.h"


TargetData AnalyzeTarget(Vector3 OwnerPos, Vector3 OwnerForward, Vector3 TargetPos) {
    Vector3 DirToTarget = (TargetPos - OwnerPos).Normalized();
    Vector3 ForwardNorm = OwnerForward.Normalized();

    double Dot = ForwardNorm.x * DirToTarget.x + ForwardNorm.y * DirToTarget.y + ForwardNorm.z * DirToTarget.z;

    if (Dot > 1.0) Dot = 1.0;
    if (Dot < -1.0) Dot = -1.0;

    double AngleDeg = std::acos(Dot) * (180.0 / 3.14159265358979323846);

    Vector3 Cross = {
        ForwardNorm.y * DirToTarget.z - ForwardNorm.z * DirToTarget.y,
        ForwardNorm.z * DirToTarget.x - ForwardNorm.x * DirToTarget.z,
        ForwardNorm.x * DirToTarget.y - ForwardNorm.y * DirToTarget.x
    };

    std::string Side = (Cross.z >= 0) ? "Right" : "Left";

    return { DirToTarget, AngleDeg, Side };
}

