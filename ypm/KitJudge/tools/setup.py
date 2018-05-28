import MySQLdb, os
import hashlib
from optparse import OptionParser

def sha256(content) :
	encoder = hashlib.sha256()
	encoder.update(content)
	return encoder.hexdigest()

KitDBHost = "localhost"
KitDBName = "KitJudge"
KitDBUserName = "root"
KitDBUserPassword = "acmclass"

KitAdminUserName = "root"
KitAdminUserPassword = sha256("acmclass")
KitAdminUserEmail = "root@acm.sjtu.edu.cn"

def kitInitDBProblem(cursor) :
	cursor.execute(
		"CREATE TABLE `KitJudge`.`KitProblem` (\
		  `kitProbId` INT NOT NULL AUTO_INCREMENT,\
		  `kitProbName` VARCHAR(64) NULL,\
		  `kitProbAccepted` INT NOT NULL DEFAULT 0,\
		  `kitProbSubmitted` INT NOT NULL DEFAULT 0,\
		  `kitProbHidden` BOOLEAN NOT NULL DEFAULT TRUE,\
		  `kitProbSource` VARCHAR(64) NULL,\
		  `kitProbType` VARCHAR(64) NOT NULL DEFAULT 'default',\
		  PRIMARY KEY (`kitProbId`)) AUTO_INCREMENT = 1000;\
		"
	)
	
def kitInitDBContest(cursor):
	cursor.execute(
		"CREATE TABLE `KitJudge`.`KitContest` (\
		  `kitContestId` INT NOT NULL AUTO_INCREMENT,\
		  `kitContestName` VARCHAR(64) NULL,\
		  `kitContestType` VARCHAR(64) NULL,\
		  `kitContestSource` VARCHAR(64) NULL,\
		  `kitContestStart` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,\
		  `kitContestDuration` INT NOT NULL,\
		  `kitContestEnd` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,\
		  `kitContestHidden` BOOLEAN NOT NULL DEFAULT TRUE,\
		  PRIMARY KEY (`kitContestId`));\
		"
	)
	cursor.execute(
		"CREATE TABLE `KitJudge`.`KitContestProblem` (\
		  `kitEntryId` INT NOT NULL AUTO_INCREMENT,\
		  `kitContestId` INT NOT NULL,\
		  `kitProbId` INT NOT NULL,\
		  `kitProbTag` VARCHAR(4) NOT NULL,\
		  PRIMARY KEY (`kitEntryId`), KEY (`kitContestId`), KEY(`kitProbTag`));\
		"
	)
    

'''
def kitInitDBJudger(cursor) :
	cursor.execute(
		"CREATE TABLE `KitJudge`.`KitJudger` (\
		  `kitJudgerId` INT NOT NULL AUTO_INCREMENT,\
		  `kitJudgerHosts` VARCHAR(32) NOT NULL,\
		  `kitJudgerStatus` INT NOT NULL,\
		  PRIMARY KEY (`kitJudgerId`));\
		"
	)
'''

def kitInitDBStatus(cursor) :
	cursor.execute(
		"CREATE TABLE `KitJudge`.`KitStatus` (\
		  `kitStatusId` INT NOT NULL AUTO_INCREMENT,\
		  `kitStatusUser` VARCHAR(64) NOT NULL,\
		  `kitStatusProbId` INT NOT NULL,\
		  `kitStatusContestId` INT NULL,\
		  `kitStatusVerdict` INT NOT NULL,\
		  `kitStatusUsedMemory` INT NULL,\
		  `kitStatusUsedTime` INT NULL,\
		  `kitStatusLanguage` VARCHAR(32) NOT NULL,\
		  `kitStatusLength` INT NOT NULL,\
		  `kitStatusTime` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,\
		  `kitStatusContestTag` VARCHAR(4) NULL,\
		  `kitStatusContestTime` INT NULL,\
		  `kitStatusScore` INT NULL,\
		  `kitStatusExtraMessage` VARCHAR(64) NULL,\
		  PRIMARY KEY (`kitStatusId`));\
		"
	)

def kitInitDBUser(cursor) :
	cursor.execute(
		"CREATE TABLE `KitJudge`.`KitUser` (\
		  `kitUserId` INT NOT NULL AUTO_INCREMENT,\
		  `kitUserName` VARCHAR(64) NULL,\
		  `kitUserPassword` VARCHAR(64) NULL,\
		  `kitUserEmail` VARCHAR(64) NOT NULL,\
		  `kitUserPriority` INT NULL,\
		  PRIMARY KEY (`kitUserId`));\
		"
	)
	sql = "INSERT INTO `KitJudge`.`KitUser` (\
		  `kitUserName`, \
		  `kitUserPassword`, \
		  `kitUserEmail`, \
		  `kitUserPriority`\
		) VALUES (\
		  '" + KitAdminUserName + "', \
		  '" + KitAdminUserPassword + "', \
		  '" + KitAdminUserEmail + "', \
		  '5'\
		);"
	cursor.execute(sql)

def kitIsDBExist(cursor, dbname) :
	result = cursor.execute("SELECT SCHEMA_NAME FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = '" + dbname + "'")
	return result
	
def kitInitMySQL() :
	conn = MySQLdb.connect(host=KitDBHost, user=KitDBUserName, passwd=KitDBUserPassword)
	cursor = conn.cursor()
	
	if (kitIsDBExist(cursor, 'KitJudge')) :
		print "WARNING: The database " + KitDBName + " has already exist."
		while (True) :
			response = raw_input("Initiatize the database?(yes/no): ")
			if (response == "yes" or response[0] == 'y') :
				cursor.execute("drop database " + KitDBName)
				break
			elif (response == "no" or response[0] == 'n') :
				os._exit(0)
			else :
				print "Invalid input."
				
	cursor.execute("create database " + KitDBName)
	cursor.execute("use " + KitDBName)
	
	kitInitDBProblem(cursor)
	kitInitDBStatus(cursor)
	kitInitDBUser(cursor)
	kitInitDBContest(cursor)
	#kitInitDBJudger(cursor)
	
	cursor.close()
	conn.commit()
	conn.close()

if __name__ == "__main__" :
	kitInitMySQL()
