#pragma once

enum class InsuranceType
{
    KRANKENVERSICHERUNG = 0,
    UNFALLVERSICHERUNG = 1,
    RENTENVERSICHERUNG = 2,
    ARBEITSLOSENVERSICHERUNG = 3,
    PFLEGEVERSICHERUNG = 4,
};

struct InsurancePercentage
{
    InsuranceType type;
    double employerShare;
    double employeeShare;
    int year;
};