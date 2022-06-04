import csv
import json
import math

#Valid ACK: Packets received on the cloud & confirmed as Acked locally --->
            #global counter is presented in the cloud & ack exist
#Invalid ACK: Packets not received on the cloud & recorded as Acked locally --->
            #global counter is not presented in the cloud & ack following the 'non-existent' global counter locally
#Valid NACK: Packets not received on the cloud & recorded as Nacked locally --->
            #global counter is not presented in the cloud & nack exist
#Incorrect NACK: Packets received on the cloud but recorded as Nacked locally ---> Helium ack - local ack
            #global counter is presented in the cloud & nack exist
import math

def extract_sequence_number_from_CSV(fileName):
    filePath = "%s.csv" % (fileName)
    sequenceNumberList = []

    with open(filePath, newline='') as csvfile:
        data = csv.reader(csvfile, delimiter=',')

        for row in data:
            if row[5]=="motion":
                sequenceNumberList.append(int(row[7]))
    return sequenceNumberList

def Data_Preprocessing(fileName):
    filePath = "%s.txt" % (fileName)
    with open(filePath, newline='') as f1:
        lines = f1.readlines()
    counter_total = math.floor(len(lines)/4) # grouped by 4
    return_list = [] # global_counter, nack/ack
    for i in range(0, counter_total):
        if lines[i*4 + 1].split(' ')[-1].strip('[').strip('\n').strip(']').isdigit():
            return_list.append([int(lines[i*4 + 1].split(' ')[-1].strip('[').strip('\n').strip(']')), lines[i*4 + 2].split(' ')[2].strip('\n')])
        else:
            print("unknown type")
            print(lines[i*4 + 1].split(' '))
            print(i)
    #print(return_list)
    temp_list = extract_sequence_number_from_CSV(fileName)
    #print(temp_list)
    return temp_list, return_list

def Ack_Nack_Validation(temp_list, return_list):
    valid_ack = 0; invalid_ack = 0; valid_nack = 0; invalid_nack = 0;
    for pair in return_list:
        if pair[0] in temp_list and pair[1] == "ACK":
            valid_ack += 1
        elif pair[0] not in temp_list and pair[1] == "ACK":
            invalid_ack += 1
        elif pair[0] not in temp_list and pair[1] == "NACK":
            valid_nack += 1
        elif pair[0] in temp_list and pair[1] == "NACK":
            invalid_nack += 1
        else:
            print("Noise")
    return [valid_ack, invalid_ack, valid_nack, invalid_nack]

if __name__ == "__main__":
    result = [0, 0, 0, 0]
    temp_list, return_list = Data_Preprocessing("1416")
    data_buffer = Ack_Nack_Validation(temp_list, return_list)
    for i in range(0, 4):
        result[i] += data_buffer[i]

    temp_list, return_list = Data_Preprocessing("1434-1543")
    data_buffer = Ack_Nack_Validation(temp_list, return_list)
    for i in range(0, 4):
        result[i] += data_buffer[i]

    temp_list, return_list = Data_Preprocessing("1642-1704")
    data_buffer = Ack_Nack_Validation(temp_list, return_list)
    for i in range(0, 4):
        result[i] += data_buffer[i]

    temp_list, return_list = Data_Preprocessing("1706-1727")
    data_buffer = Ack_Nack_Validation(temp_list, return_list)
    for i in range(0, 4):
        result[i] += data_buffer[i]

    print("[valid_ack, invalid_ack, valid_nack, invalid_nack]: ", result)
    print("Valid Ack rate:", result[0]/(result[0]+result[1] + result[2] + result[3]))
    print("Invalid Ack rate: ", result[1]/(result[0]+result[1] + result[2] + result[3]))
    print("Valid Nack rate: ", result[2] / (result[0]+result[1] + result[2] + result[3]))
    print("Invalid Nack rate: ", result[3] / (result[0]+result[1] + result[2] + result[3]))