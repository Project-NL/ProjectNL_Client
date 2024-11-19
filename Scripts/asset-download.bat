@echo off
FOR %%A IN ("%cd%") DO aws s3 cp s3://project-nl-content/Assets %%~dpAContent\Assets --recursive