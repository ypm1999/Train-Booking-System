CREATE DATABASE  IF NOT EXISTS `mydb` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `mydb`;
-- MySQL dump 10.13  Distrib 5.5.46, for debian-linux-gnu (x86_64)
--
-- Host: 127.0.0.1    Database: mydb
-- ------------------------------------------------------
-- Server version	5.5.46-0ubuntu0.14.04.2

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `django_migrations`
--

DROP TABLE IF EXISTS `django_migrations`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `django_migrations` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `app` varchar(255) NOT NULL,
  `name` varchar(255) NOT NULL,
  `applied` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=21 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `django_migrations`
--

LOCK TABLES `django_migrations` WRITE;
/*!40000 ALTER TABLE `django_migrations` DISABLE KEYS */;
INSERT INTO `django_migrations` VALUES (1,'books','0001_initial','2015-12-27 06:17:00'),(2,'books','0002_auto_20151226_1522','2015-12-27 06:17:00'),(3,'order_ticket','0001_initial','2015-12-27 06:17:00'),(4,'contenttypes','0001_initial','2015-12-28 05:11:53'),(5,'auth','0001_initial','2015-12-28 05:11:53'),(6,'admin','0001_initial','2015-12-28 05:11:53'),(7,'contenttypes','0002_remove_content_type_name','2015-12-28 05:11:54'),(8,'auth','0002_alter_permission_name_max_length','2015-12-28 05:11:54'),(9,'auth','0003_alter_user_email_max_length','2015-12-28 05:11:54'),(10,'auth','0004_alter_user_username_opts','2015-12-28 05:11:54'),(11,'auth','0005_alter_user_last_login_null','2015-12-28 05:11:54'),(12,'auth','0006_require_contenttypes_0002','2015-12-28 05:11:54'),(13,'order_ticket','0002_auto_20151228_0510','2015-12-28 05:11:54'),(14,'sessions','0001_initial','2015-12-28 05:11:54'),(15,'order_ticket','0003_train_seat','2015-12-28 07:18:21'),(16,'order_ticket','0004_ticketorder','2015-12-28 10:23:53'),(17,'mylogin','0001_initial','2015-12-29 06:42:06'),(18,'order_ticket','0005_auto_20151229_0832','2015-12-29 08:32:12'),(19,'mylogin','0002_passenger_name','2015-12-29 09:48:00'),(20,'order_ticket','0006_ticketorder_order_time','2015-12-29 10:08:36');
/*!40000 ALTER TABLE `django_migrations` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2015-12-29  4:40:52
