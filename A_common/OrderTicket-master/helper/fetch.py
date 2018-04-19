from BeautifulSoup import BeautifulSoup as bs
import requests as rq
l = open("beijingnan.txt").read().split("\n")
se = rq.Session()
for i in l:
    if i.find("G") != -1:
        url = "http://www.huochepiao.com" + i
        soup = bs(se.get(url).content)
        tab = soup.findAll("table")
        info = tab[0].findAll("tr")[0].findAll("td")
        train = info[2].contents[0]
        cost_hours = info[4].contents[0]
        time = tab[0].findAll("tr")[2].findAll("td")
        st_time = time[1].contents[0]
        ed_time = time[3].contents[0]
        print train,cost_hours,st_time,ed_time,"|",
        tr = tab[1].findAll("tr")[1:]
        for j in tr:
            td = j.findAll("td")
            train = td[0].contents[0]
            order = td[1].contents[0]
            station = td[2].findAll("a")[0].contents[0]
            time = td[4].contents[0]
            cost = td[9].contents
            if cost == []:
                cost = [0]
            cost = cost[0]
            print train,order,station,time,cost,"|",
        print ""
    

