#!/usr/bin/env bash

rm allFiles/*.csv
cp datasets/gdp_per_capita.csv allFiles
cp datasets/ddf--datapoints--poisonings_deaths_per_100000_people.csv allFiles
cp datasets/ddf--datapoints--vacc_rate--by--country--time.csv allFiles
cp datasets/ddf--datapoints--females_aged_15plus_employment_rate_percent--by--geo--time.csv allFiles
cp datasets/ddf--datapoints--females_aged_15plus_labour_force_participation_rate_percent--by--geo--time.csv allFiles
cp datasets/ddf--datapoints--females_aged_15plus_unemployment_rate_percent--by--geo--time.csv allFiles
cp datasets/ddf--datapoints--females_aged_25_54_labour_force_participation_rate_percent--by--geo--time.csv allFiles
cp datasets/ddf--datapoints--females_aged_25_54_unemployment_rate_percent--by--geo--time.csv allFiles
#cp datasets/ddf--datapoints--females_aged_25_64_unemployment_rate_percent--by--geo--time.csv allFiles
