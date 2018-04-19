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
-- Table structure for table `order_ticket_ticketpool`
--

DROP TABLE IF EXISTS `order_ticket_ticketpool`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `order_ticket_ticketpool` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `date` date NOT NULL,
  `st_order` int(11) NOT NULL,
  `ed_order` int(11) NOT NULL,
  `number` int(11) NOT NULL,
  `train_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `order_ticket_ticketpool_f78b5793` (`train_id`),
  CONSTRAINT `order_ticket__train_id_52b334e319bd2532_fk_order_ticket_train_id` FOREIGN KEY (`train_id`) REFERENCES `order_ticket_train` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=151 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `order_ticket_ticketpool`
--

LOCK TABLES `order_ticket_ticketpool` WRITE;
/*!40000 ALTER TABLE `order_ticket_ticketpool` DISABLE KEYS */;
INSERT INTO `order_ticket_ticketpool` VALUES (1,'2015-12-01',1,9,2,29),(5,'2015-12-01',1,4,1,29),(6,'2015-12-01',2,9,1,29),(7,'2015-12-01',1,18,50,118),(8,'2015-12-01',1,16,50,113),(9,'2015-12-01',1,10,50,2),(10,'2015-12-01',1,21,50,122),(11,'2015-12-01',1,21,50,99),(12,'2015-12-01',1,10,50,3),(13,'2015-12-01',1,14,50,4),(14,'2015-12-01',1,10,50,5),(15,'2015-12-01',1,25,50,104),(16,'2015-12-01',1,14,50,6),(17,'2015-12-01',1,8,50,7),(18,'2015-12-01',1,16,50,114),(19,'2015-12-01',1,10,50,8),(20,'2015-12-01',1,6,50,100),(21,'2015-12-01',1,27,50,105),(22,'2015-12-01',1,13,50,9),(23,'2015-12-01',1,14,0,10),(24,'2015-12-01',1,17,50,108),(25,'2015-12-01',1,21,50,102),(26,'2015-12-01',1,12,50,101),(27,'2015-12-01',1,3,50,11),(28,'2015-12-01',1,6,50,12),(29,'2015-12-01',1,7,50,13),(30,'2015-12-01',1,18,49,14),(31,'2015-12-01',1,27,50,142),(32,'2015-12-01',1,9,50,15),(33,'2015-12-01',1,14,50,120),(34,'2015-12-01',1,17,50,123),(35,'2015-12-01',1,11,50,16),(36,'2015-12-01',1,6,50,17),(37,'2015-12-01',1,5,50,18),(38,'2015-12-01',1,10,50,19),(39,'2015-12-01',1,25,49,20),(40,'2015-12-01',1,7,50,21),(41,'2015-12-01',1,21,50,126),(42,'2015-12-01',1,12,50,103),(43,'2015-12-01',1,3,50,22),(44,'2015-12-01',1,25,50,110),(45,'2015-12-01',1,8,50,23),(46,'2015-12-01',1,15,48,24),(47,'2015-12-01',1,10,50,25),(48,'2015-12-01',1,14,50,124),(49,'2015-12-01',1,8,50,26),(50,'2015-12-01',1,27,50,111),(51,'2015-12-01',1,24,50,125),(52,'2015-12-01',1,9,50,27),(53,'2015-12-01',1,27,50,140),(54,'2015-12-01',1,10,50,28),(55,'2015-12-01',1,22,50,106),(56,'2015-12-01',1,9,50,29),(57,'2015-12-01',1,9,50,107),(58,'2015-12-01',1,9,50,112),(59,'2015-12-01',1,4,50,30),(60,'2015-12-01',1,10,50,31),(61,'2015-12-01',1,28,50,32),(62,'2015-12-01',1,16,50,128),(63,'2015-12-01',1,12,50,33),(64,'2015-12-01',1,15,50,109),(65,'2015-12-01',1,22,50,34),(66,'2015-12-01',1,9,50,35),(67,'2015-12-01',1,26,50,119),(68,'2015-12-01',1,4,50,36),(69,'2015-12-01',1,10,50,37),(70,'2015-12-01',1,11,50,38),(71,'2015-12-01',1,19,50,39),(72,'2015-12-01',1,11,50,40),(73,'2015-12-01',1,10,50,41),(74,'2015-12-01',1,10,50,42),(75,'2015-12-01',1,22,50,43),(76,'2015-12-01',1,22,50,115),(77,'2015-12-01',1,9,50,44),(78,'2015-12-01',1,11,50,45),(79,'2015-12-01',1,10,50,116),(80,'2015-12-01',1,21,49,46),(81,'2015-12-01',1,7,50,47),(82,'2015-12-01',1,10,50,117),(83,'2015-12-01',1,8,50,48),(84,'2015-12-01',1,11,50,49),(85,'2015-12-01',1,18,50,50),(86,'2015-12-01',1,16,50,137),(87,'2015-12-01',1,14,50,121),(88,'2015-12-01',1,12,50,51),(89,'2015-12-01',1,9,50,52),(90,'2015-12-01',1,3,50,53),(91,'2015-12-01',1,18,50,129),(92,'2015-12-01',1,4,50,54),(93,'2015-12-01',1,15,50,55),(94,'2015-12-01',1,9,50,56),(95,'2015-12-01',1,12,50,57),(96,'2015-12-01',1,3,50,58),(97,'2015-12-01',1,10,50,131),(98,'2015-12-01',1,13,50,59),(99,'2015-12-01',1,17,50,130),(100,'2015-12-01',1,10,50,60),(101,'2015-12-01',1,8,50,61),(102,'2015-12-01',1,3,50,62),(103,'2015-12-01',1,8,50,63),(104,'2015-12-01',1,10,50,64),(105,'2015-12-01',1,10,50,65),(106,'2015-12-01',1,10,50,66),(107,'2015-12-01',1,19,50,127),(108,'2015-12-01',1,21,50,132),(109,'2015-12-01',1,4,50,67),(110,'2015-12-01',1,17,50,133),(111,'2015-12-01',1,13,50,68),(112,'2015-12-01',1,11,50,69),(113,'2015-12-01',1,11,50,70),(114,'2015-12-01',1,12,50,71),(115,'2015-12-01',1,14,50,72),(116,'2015-12-01',1,16,50,73),(117,'2015-12-01',1,18,50,134),(118,'2015-12-01',1,16,50,74),(119,'2015-12-01',1,15,50,135),(120,'2015-12-01',1,10,50,75),(121,'2015-12-01',1,6,50,76),(122,'2015-12-01',1,11,50,77),(123,'2015-12-01',1,13,50,78),(124,'2015-12-01',1,16,50,138),(125,'2015-12-01',1,12,50,79),(126,'2015-12-01',1,11,50,80),(127,'2015-12-01',1,9,50,81),(128,'2015-12-01',1,12,50,82),(129,'2015-12-01',1,10,50,83),(130,'2015-12-01',1,3,50,84),(131,'2015-12-01',1,13,50,141),(132,'2015-12-01',1,9,50,85),(133,'2015-12-01',1,11,50,86),(134,'2015-12-01',1,9,50,87),(135,'2015-12-01',1,10,50,88),(136,'2015-12-01',1,12,50,136),(137,'2015-12-01',1,9,50,89),(138,'2015-12-01',1,9,50,90),(139,'2015-12-01',1,4,50,91),(140,'2015-12-01',1,8,50,92),(141,'2015-12-01',1,7,50,93),(142,'2015-12-01',1,7,50,94),(143,'2015-12-01',1,8,50,95),(144,'2015-12-01',1,9,50,144),(145,'2015-12-01',1,4,50,96),(146,'2015-12-01',1,4,50,97),(147,'2015-12-01',1,2,50,98),(148,'2015-12-01',4,15,2,24),(149,'2015-12-01',21,25,1,20),(150,'2015-12-01',4,14,4,10);
/*!40000 ALTER TABLE `order_ticket_ticketpool` ENABLE KEYS */;
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
