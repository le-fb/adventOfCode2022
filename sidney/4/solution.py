
import math


file1 = open('./sidney/4/data.txt', 'r')
lines = file1.read().splitlines()
file1.close()

full_overlap_count = 0
for line in lines:
    
    partners = line.split(',')
    partner_1 = partners[0].split('-')
    partner_2 = partners[1].split('-')

    lower_partner_1 = int(partner_1[0])
    upper_partner_1 = int(partner_1[1])

    lower_partner_2 = int(partner_2[0])
    upper_partner_2 = int(partner_2[1])

    if lower_partner_1 <= lower_partner_2 and upper_partner_2 <= upper_partner_1:
        full_overlap_count += 1
    elif lower_partner_2 <= lower_partner_1 and upper_partner_1 <= upper_partner_2:
        full_overlap_count += 1

print(f'{full_overlap_count} pairs fully overlap')

partial_overlap_count = 0
for line in lines:
    
    partners = line.split(',')
    partner_1 = partners[0].split('-')
    partner_2 = partners[1].split('-')

    lower_partner_1 = int(partner_1[0])
    upper_partner_1 = int(partner_1[1])

    lower_partner_2 = int(partner_2[0])
    upper_partner_2 = int(partner_2[1])

    if lower_partner_2 >= lower_partner_1 and lower_partner_2 <= upper_partner_1:
        partial_overlap_count += 1
    elif lower_partner_1 >= lower_partner_2 and lower_partner_1 <= upper_partner_2:
        partial_overlap_count += 1

print(f'{partial_overlap_count} pairs partially overlap')