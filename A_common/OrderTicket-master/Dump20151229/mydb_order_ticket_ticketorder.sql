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
-- Table structure for table `order_ticket_ticketorder`
--

DROP TABLE IF EXISTS `order_ticket_ticketorder`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `order_ticket_ticketorder` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `date` date NOT NULL,
  `st` varchar(30) NOT NULL,
  `ed` varchar(30) NOT NULL,
  `cost` double NOT NULL,
  `train_id` int(11) NOT NULL,
  `user_id` int(11) NOT NULL,
  `order_time` datetime NOT NULL,
  PRIMARY KEY (`id`),
  KEY `order_ticket__train_id_38096bc2ce83d92f_fk_order_ticket_train_id` (`train_id`),
  KEY `order_ticket_ti_user_id_423de92b2765f819_fk_mylogin_passenger_id` (`user_id`),
  CONSTRAINT `order_ticket_ti_user_id_423de92b2765f819_fk_mylogin_passenger_id` FOREIGN KEY (`user_id`) REFERENCES `mylogin_passenger` (`id`),
  CONSTRAINT `order_ticket__train_id_38096bc2ce83d92f_fk_order_ticket_train_id` FOREIGN KEY (`train_id`) REFERENCES `order_ticket_train` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=25 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `order_ticket_ticketorder`
--

LOCK TABLES `order_ticket_ticketorder` WRITE;
/*!40000 ALTER TABLE `order_ticket_ticketorder` DISABLE KEYS */;
INSERT INTO `order_ticket_ticketorder` VALUES (1,'2015-12-01','北京南','福州',719,29,1,'2015-12-29 10:08:30'),(2,'2015-12-01','北京南','福州',719,29,1,'2015-12-29 10:08:30'),(3,'2015-12-01','北京南','福州',719,29,1,'2015-12-29 10:08:30'),(4,'2015-12-01','北京南','福州',719,29,1,'2015-12-29 10:08:30'),(5,'2015-12-01','北京南','福州',719,29,1,'2015-12-29 10:08:30'),(6,'2015-12-01','北京南','福州',719,29,1,'2015-12-29 10:08:30'),(7,'2015-12-01','北京南','福州',719,29,1,'2015-12-29 10:08:30'),(8,'2015-12-01','北京南','福州',719,29,1,'2015-12-29 10:08:30'),(9,'2015-12-01','北京南','福州',719,29,1,'2015-12-29 10:08:30'),(10,'2015-12-01','北京南','福州',719,29,1,'2015-12-29 10:08:30'),(11,'2015-12-01','北京南','福州',719,29,1,'2015-12-29 10:08:30'),(12,'2015-12-01','北京南','福州',719,29,2,'2015-12-29 10:08:30'),(13,'2015-12-01','北京南','福州',719,29,2,'2015-12-29 10:08:30'),(14,'2015-12-01','北京南','福州',719,29,2,'2015-12-29 10:09:58'),(15,'2015-12-01','北京南','福州',719,29,2,'2015-12-29 10:11:21'),(16,'2015-12-01','北京南','唐山',110.5,24,3,'2015-12-29 10:27:29'),(17,'2015-12-01','北京南','福州',0,46,1,'2015-12-29 10:31:31'),(18,'2015-12-01','北京南','福州',719,20,1,'2015-12-29 10:42:17'),(19,'2015-12-01','北京南','唐山',110.5,24,2,'2015-12-29 11:15:52'),(20,'2015-12-01','北京南','唐山',0,10,2,'2015-12-29 11:22:36'),(21,'2015-12-01','北京南','唐山',0,10,2,'2015-12-29 11:22:38'),(22,'2015-12-01','北京南','唐山',0,10,2,'2015-12-29 11:22:41'),(23,'2015-12-01','北京南','唐山',0,10,2,'2015-12-29 11:22:43'),(24,'2015-12-01','北京南','福州',765,14,2,'2015-12-29 12:39:38');
/*!40000 ALTER TABLE `order_ticket_ticketorder` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2015-12-29  4:40:53
