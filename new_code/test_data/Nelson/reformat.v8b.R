# Erik Nelson's data, take 2
# version 8
# Thu Jan 14 07:02:48 2016
#
# CBC, Mon Mar  6 13:55:12 2017
# updated COMMENTS ONLY to pass to Will
#   (some of the comments still refer to V7 of this code)
#
#
# This version is meant to create distinct data sets for tropical and temperate
#
#
# For this set of data
# 1. Run WEKA w/o the quantiles for the class attributes
#    with the default setting, these were not actually evenly distributed
#      the issue is the large number of values that cluster around 0
#      it might be that letting WEKA pick the ranges is the best idea
#    I did check the class attribute, and that comes out fine
# 2. Class attribute will be H/M/L -- for both tons and kcals
#      change from year before
#      this does have the M class centered around 0
# 3. Other attributes will be difference from year before
# 4. Vary these things:
#    tons/kcals                        (both H/M/L)
#    with year / without year          (attribute included or not)
#    with tropics / without tropics    (attribute included or not)
# 5. File names:
#      nelson.3c.t.wy.wt.arff, e.g.
#      nelson.3c.k.ny.ny.arff, e.g.
#      2 versions of class * 2 years * 2 tropics == 8 versions
#
# V8 update:
# files are named:
#   first eight: nelson.3c.[t|k].[wy|ny].[wt|nt]
#   second eight don't have wt|nt, but are separated into
#     "temperate" (temp) or "tropics" (trop)

############################################################
#overall:
# read in the data
# create additional columns for differences
# remove every Nth row
# --> core data sheet, then
#
# 1. Change just class attribute to H/L
#   times 2, for tons/ha and Mkcals/ha
# 2. Try also with H/M/L for class attributes ... maybe also 4, or 5?
#   times 2
# 3. Convert features to H/M/L (not for this version)

# Note to Will: We played around with two classes or four, but stuck with three


############################################################
#back to working on diffs...
#http://stackoverflow.com/questions/21667262/how-to-find-difference-between-values-in-two-rows-in-an-r-dataframe-using-dplyr
#http://blog.rstudio.org/2014/01/17/introducing-dplyr/
install.packages('plyr')  # for mapvalues
library(plyr)             # note that this masks some dpylr methods
install.packages('dplyr')
library(dplyr)
require(dplyr)  # i don't know what this does or if it's actually needed

install.packages("gtools") # for quantcut
library(gtools)

# Saving this info, FYI:
#The following objects are masked from ‘package:plyr’:
#  arrange, count, desc, failwith, id, mutate, rename, summarise, summarize
#The following objects are masked from ‘package:stats’:
#  filter, lag
#The following objects are masked from ‘package:base’:
#  intersect, setdiff, setequal, union

############################################################
#Read in the data
#  note that I changed headers, which had / in them and R didn't like that
data <- read.csv("Nelson.v2.csv", head=TRUE, stringsAsFactors=FALSE)


# I don't know exactly what this syntax does; all applies to data variable
#   and then gets stored in dataNew
#   including the new "diff" columns" with the difference from the year before
# orignal variables: soilscore, ha, rice, wheat, sugar, grains, oil, fruits, roots, other,
#                    davg, navg, pavg, irr, landd, eqp
dataNew = data %>%
  group_by(id) %>%
  mutate(tonsDiff = tons_ha-lag(tons_ha, default=0)) %>%
  mutate(BcalsDiff = Bcals_ha-lag(Bcals_ha, default=0)) %>%
  mutate(soilDiff = soilscore-lag(soilscore, default=0)) %>%
  mutate(haDiff = ha-lag(ha, default=0)) %>%
  mutate(riceDiff = rice-lag(rice, default=0)) %>%
  mutate(wheatDiff = wheat-lag(wheat, default=0)) %>%
  mutate(sugarDiff = sugar-lag(sugar, default=0)) %>%
  mutate(grainsDiff = grains-lag(grains, default=0)) %>%
  mutate(oilDiff = oil-lag(oil, default=0)) %>%
  mutate(fruitsDiff = fruits-lag(fruits, default=0)) %>%
  mutate(rootsDiff = roots-lag(roots, default=0)) %>%
  mutate(otherDiff = other-lag(other, default=0)) %>%
  mutate(davgDiff = davg-lag(davg, default=0)) %>%
  mutate(navgDiff = navg-lag(navg, default=0)) %>%
  mutate(pavgDiff = pavg-lag(pavg, default=0)) %>%
  mutate(irrDiff = irr-lag(irr, default=0)) %>%
  mutate(landdDiff = landd-lag(landd, default=0)) %>%
  mutate(eqpDiff = eqp-lag(eqp, default=0))

# need to delete the first row for each country
#   see this:
# http://stackoverflow.com/questions/7942519/deleting-every-n-th-row-in-a-dataframe

# CBC: modified to remove row mod n
# since R is 1-indexed, not 0-indexed, use mod=1
Nth.delete<-function(dataframe,n) {
  r<- 1:nrow(dataframe)
  dataframe[!(r%%n==1),]
}
#Nth.delete(DF, 3)
dataNew2 <- Nth.delete(dataNew, 33)      ## 33 years --> 32 diffs
#write.csv(dataNew2, 'changes.csv')

############################################################
# add class attribute to H/L tons/HA
#class <- quantcut(dataNew2$tonsDiff, q=seq(0,1,by=1/2))
#dataNew2$tHA_2 <-  mapvalues(class, from=levels(class), to=c("L", "H"))
class <- quantcut(dataNew2$tonsDiff, q=seq(0,1,by=1/3))
dataNew2$tHA_3 <-  mapvalues(class, from=levels(class), to=c("L", "M", "H"))
#class <- quantcut(dataNew2$tonsDiff, q=seq(0,1,by=1/4))
#dataNew2$tHA_4 <-  mapvalues(class, from=levels(class), to=c("L", "ML", "MH", "H"))

############################################################
# add class attribute to H/L BillionCals/HA
#class <- quantcut(dataNew2$BcalsDiff, q=seq(0,1,by=1/2))
#dataNew2$BcHA_2 <-  mapvalues(class, from=levels(class), to=c("L", "H"))
class <- quantcut(dataNew2$BcalsDiff, q=seq(0,1,by=1/3))
dataNew2$BcHA_3 <-  mapvalues(class, from=levels(class), to=c("L", "M", "H"))
#class <- quantcut(dataNew2$BcalsDiff, q=seq(0,1,by=1/4))
#dataNew2$BcHA_4 <-  mapvalues(class, from=levels(class), to=c("L", "ML", "MH", "H"))

write.csv(dataNew2, 'changes.check.csv')

# remove the original features
# http://stackoverflow.com/questions/4605206/drop-columns-in-r-data-frame
#df <- subset(df, select = -c(a,c) )
dataNew3 <- subset(dataNew2, select= -c(tons_ha, Bcals_ha, tonsDiff, BcalsDiff,
         soilscore, ha, rice, wheat, sugar, grains, oil, fruits, roots, other,
         davg, navg, pavg, irr, landd, eqp))
#reorder columns
# OOPS! need to put class attributes first
dataNew3 <- dataNew3[c("tHA_3", "BcHA_3",
     "id", "year", "tropical",
     "soilDiff", "haDiff", "riceDiff", "wheatDiff", "sugarDiff", "grainsDiff",
     "oilDiff", "fruitsDiff", "rootsDiff", "otherDiff", "davgDiff",
     "navgDiff", "pavgDiff", "irrDiff", "landdDiff", "eqpDiff")]
write.csv(dataNew3, 'changes.diffs.csv')

#### (dataNew4, with features as tertiles, removed in this version)

# for new output, January 14
# start with dataNew3
#   fork to tons / kcals
#   fork those to year / no year
#   fork those to tropics / no tropics
# ? what about ID? I think I should look at this... later

#write nelson.3c.t.wy.wt.arff, nelson.3c.k.wy.wt.arff


# remove id
# remove one of tons / kcals
# write to arff file, write nelson.3c.t.wy.wt.arff, nelson.3c.k.wy.wt.arff

library(foreign)

# year, topics
nelson.t <- subset(dataNew3, select=-c(id,BcHA_3))
nelson.k <- subset(dataNew3, select=-c(id,tHA_3))

# no year, but tropics
nelson.t.ny <- subset(nelson.t, select=-c(year))
nelson.k.ny <- subset(nelson.k, select=-c(year))

# year, but no tropics
nelson.t.y.nt <- subset(nelson.t, select=-c(tropical))
nelson.k.y.nt <- subset(nelson.k, select=-c(tropical))

# no year, no tropics
nelson.t.ny.nt <-subset(nelson.t.ny, select=-c(tropical))
nelson.k.ny.nt <-subset(nelson.k.ny, select=-c(tropical))

# write 8 files
write.arff(nelson.t,       'nelson.3c.t.wy.wt.arff')
write.arff(nelson.k,       'nelson.3c.k.wy.wt.arff')
write.arff(nelson.t.ny,    'nelson.3c.t.ny.wt.arff')
write.arff(nelson.k.ny,    'nelson.3c.k.ny.wt.arff')
write.arff(nelson.t.y.nt,  'nelson.3c.t.wy.nt.arff')
write.arff(nelson.k.y.nt,  'nelson.3c.k.wy.nt.arff')
write.arff(nelson.t.ny.nt, 'nelson.3c.t.ny.nt.arff')
write.arff(nelson.k.ny.nt, 'nelson.3c.k.ny.nt.arff')

# now, write different datasets for temperate/tropical
# try subset(dftm, C!="Foo") syntax
# need both tons and kcals; need both years and no years; need data that has tropical column
# so that's the first four: nelson.t, nelson.k, nelson.t.ny, nelson.k.ny
# split all four of these into two different outputfiles

nelson.t.temperate <- subset(nelson.t, tropical!=0)
nelson.t.tropical <- subset(nelson.t, tropical==0)
nelson.k.temperate <- subset(nelson.k, tropical!=0)
nelson.k.tropical <- subset(nelson.k, tropical==0)

nelson.t.ny.temperate <- subset(nelson.t.ny, tropical!=0)
nelson.t.ny.tropical <- subset(nelson.t.ny, tropical==0)
nelson.k.ny.temperate <- subset(nelson.k.ny, tropical!=0)
nelson.k.ny.tropical <- subset(nelson.k.ny, tropical==0)

# write 8 files
write.arff(nelson.t.temperate,       'nelson.3c.t.wy.temp.arff')
write.arff(nelson.t.tropical,        'nelson.3c.t.wy.trop.arff')
write.arff(nelson.k.temperate,       'nelson.3c.k.wy.temp.arff')
write.arff(nelson.k.tropical,        'nelson.3c.k.wy.trop.arff')

write.arff(nelson.t.ny.temperate,    'nelson.3c.t.ny.temp.arff')
write.arff(nelson.t.ny.tropical,     'nelson.3c.t.ny.trop.arff')
write.arff(nelson.k.ny.temperate,    'nelson.3c.k.ny.temp.arff')
write.arff(nelson.k.ny.tropical,     'nelson.3c.k.ny.trop.arff')
