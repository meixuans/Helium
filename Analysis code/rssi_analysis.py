import csv
import numpy as np
import matplotlib.pyplot as plt

def drawlastDay_cdf():
    rssi_value=[]
    with open('./lastDay.csv',newline='') as csvfile:
        data=csv.reader(csvfile, delimiter=',')
        for row in data:
            if row[5]=="rssi":
                print(row[7])
                rssi_value.append(float(row[7]))
    # sort in ascending
    rssi_sorted=sorted(rssi_value)
    length=len(rssi_sorted) #the number of points
    # generate a  histogram to gain the data list
    new_list,edge=np.histogram(rssi_sorted, bins=30)
    # normalize
    pdf = new_list/ sum(new_list)
    # cumulative
    cdf = np.cumsum(pdf)
    # figure CDF
    plt.plot(edge[1:],cdf, label="CDF")
    plt.legend()
    plt.show()
    # figure histogram
    plt.hist(rssi_sorted, bins='auto')
    plt.title("Histogram with 'auto' bins")
    plt.show()


def drawlastMonth_cdf():
    rssi_value=[]
    with open('./lastMonth.csv',newline='') as csvfile:
        data=csv.reader(csvfile, delimiter=',')
        for row in data:
            if row[5]=="rssi":
                print(row[7])
                rssi_value.append(float(row[7]))
    # sort in ascending
    rssi_sorted=sorted(rssi_value)
    length=len(rssi_sorted) #the number of points
    # generate a  histogram to gain the data list,bins can be customized
    new_list,edge=np.histogram(rssi_sorted, bins=30)
    # normalize
    pdf = new_list/ sum(new_list)
    # cumulative
    cdf = np.cumsum(pdf)
    # figure CDF
    plt.plot(edge[1:],cdf, label="CDF")
    plt.legend()
    plt.show()
    # figure histogram
    plt.hist(rssi_sorted, bins='auto')
    plt.title("Histogram with 'auto' bins")
    plt.show()



# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    drawlastDay_cdf()
    drawlastMonth_cdf()

# See PyCharm help at https://www.jetbrains.com/help/pycharm/
