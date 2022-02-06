// Fill out your copyright notice in the Description page of Project Settings.


#include "SaberUtils.h"

FVector SaberUtils::GetOrthogonalVectorPointToPlane(FVector Point, FVector PointOnPlane, FVector PlaneNormal)
{
	PlaneNormal.Normalize();
	float Distance = -1 * SignedDistancePointToPlane(Point, PointOnPlane, PlaneNormal);
	return PlaneNormal * Distance;
}
