#!/usr/bin/R
library(nortest)
x <- read.table("rwxf",sep='\n')
shapiro.test(x$V1)
ad.test(x$V1)
