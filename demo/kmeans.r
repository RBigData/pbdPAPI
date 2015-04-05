library(pbdPAPI)
library(scribe)
rm(list = ls())

resample <- sample(1:nrow(iris), size=197000, replace=TRUE)
iris <- iris[resample, ]

id <- as.integer(iris[, 5])
X <- as.matrix(iris[, -5])
Mu <- t(sapply(1:3, function(k) colMeans(X[id == k,])))

### Profile K-means
m1.flops <- system.flops( m.1 <- kmeans(X, Mu, iter.max = n.iter, nstart = 1) )
m2.flops <- system.flops( m.2 <- kmeans(X, Mu, iter.max = n.iter, nstart = 1,
                                        algorithm = "Lloyd") )
m3.flops <- system.flops( m.3 <- kmeans(X, Mu, iter.max = n.iter, nstart = 1,
                                        algorithm = "Forgy") )
m4.flops <- system.flops( m.4 <- kmeans(X, Mu, iter.max = n.iter, nstart = 1,
                                        algorithm = "MacQueen") )

m1.flops
m2.flops
m3.flops
m4.flops

m1.cm <- system.cache( m.1 <- kmeans(X, Mu, iter.max = n.iter, nstart = 1) )
m2.cm <- system.cache( m.2 <- kmeans(X, Mu, iter.max = n.iter, nstart = 1,
                                     algorithm = "Lloyd") )
m3.cm <- system.cache( m.3 <- kmeans(X, Mu, iter.max = n.iter, nstart = 1,
                                     algorithm = "Forgy") )
m4.cm <- system.cache( m.4 <- kmeans(X, Mu, iter.max = n.iter, nstart = 1,
                                     algorithm = "MacQueen") )

plot(m1.cm, m2.cm, m3.cm, m4.cm,
     opnames=c("Hartigan-Wong", "Lloyd", "Forgy", "MacQueen")) + 
    ggtitle(paste0("Comparing the Performance of K-Means Algorithms\non ",
            memuse::object.size(X), " of data"))
