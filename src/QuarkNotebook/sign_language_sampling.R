# install.packages("dplyr")
library(dplyr)

setwd("C:/Workspace/ml/quark/src/QuarkNotebook")

data <- read.table("./datasets/table_sign_complete.csv", sep=",", dec= ".")
summary(data)

data_training_sample <- data[sample(nrow(data), 250), ]
data_testing_sample <- anti_join(data, data_training_sample)

write.csv(data_training_sample, file = "./datasets/table_sign_training.csv", row.names = FALSE)
write.csv(data_testing_sample, file = "./datasets/table_sign_validation.csv", row.names = FALSE)