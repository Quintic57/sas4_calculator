'''
TODO:   Take entire html table and loop through each row. Check if the name (the first 'td' element) matches one of the attributes
        in the predefined item dictionary; if it does, update its value in the dictionary. Once done, write the updated item dictionary
        to an outputted CSV file based upon the weapon category. NOTE: the item dictionary has the NAME attribute, which is always the second 
        row in the entire table (i.e. 'tr' 1 in the table array)
'''

from bs4 import BeautifulSoup
import csv
import os
import urllib.request

baseURL = 'https://saszombieassault.fandom.com/wiki/';
weapon_dictionary = {'assault_rifles': [], 'disc_throwers': [], 'flamethrowers': [], 'lasers': [], 'lmgs': [], 'pistols': [], 'rocket_launchers': [], 'smgs': [], 'shotguns': [], 'sniper_rifles': []}
weapon_csv_header = ['Name', 'Damage/Pellet', 'Pierce', 'Pellets/shot', 'Rate of Fire', 'Capacity', 'Reload Time']

def main():
    # Populate Weapons List with all weapons from each category
    weapon_list_html = getSoup('Weapons')
    weapon_list_html_sas4 = weapon_list_html.find_all('h2')[5].find_all_next('h3')
    for h3 in weapon_list_html_sas4:
        weapon_category = h3.find('span')
        if (type(weapon_category) is not type(None)):
            weapon_category = weapon_category['id'].replace('_3', '').lower()
            if (weapon_category in weapon_dictionary):
                weapon_category_list = h3.find_next('div').find('ul').find_all('li')
                print(weapon_category)
                # All weapon categories except disc throwers and lasers use the div tag
                if (weapon_category == 'disc_throwers' or weapon_category == 'lasers'):
                    weapon_category_list = [h3.find_next('ul').find('li')]

                for weapon in weapon_category_list:
                    weapon = weapon.find('a').string
                    weapon_dictionary.get(weapon_category).append(weapon)

    initCSV()

    # Run through all weapons for each category
    for weapon_category in weapon_dictionary.values():
        for weapon_name in weapon_category:
            print ('Current Item: ' + weapon_name)
            item = {'Name': weapon_name, 'Damage/Pellet': [0, 0, 0],'Pierce': [0, 0, 0], 'Pellets/shot': 1, 'Rate of Fire': 0, 'Capacity': 0, 'Reload Time': 0, 'Weapon Class': ''}
            
            try:
                # Replaced all spaces with underscores to get the appropriate web page
                page = getSoup(weapon_name.replace(' ', '_'))
                # Get display table
                weapon_table = page.find_all('table')[0]
                '''
                Rate of Fire, Capacity, Reload Time each have two 'td' values for their
                respective rows. The first 'td' is the attribute name, the second is the attribute value

                Pierce and Damage/Pellet can have different values between NORMAL/RED/BLACK versions

                Pellets/shot isn't even on the same html level... fucking FANDOM wikia
                '''
                for row in weapon_table.find_all('tr'):
                    temp = row.find_all('td')
                    if (len(temp) > 0):
                        if (type(temp[0].string) is not type(None) and temp[0].string.strip() in item):
                            if (temp[0].string.strip() == 'Pierce'):
                                if (len(temp[1].contents) > 1):
                                    item.get('Pierce')[0] = float(temp[1].contents[0])
                                    item.get('Pierce')[1] = float(temp[1].contents[2])
                                    item.get('Pierce')[2] = float(temp[1].contents[2])
                                else:
                                    
                                    item.update({'Pierce': [float(temp[1].contents[0]) for i in range(0,3)]})
                            elif (temp[0].string.strip() == 'Damage/Pellet'):
                                if (len(temp[1].contents) > 2):
                                    item.get('Damage/Pellet')[0] = float(temp[1].contents[0].replace(',', ''))
                                    item.get('Damage/Pellet')[1] = float(temp[1].contents[2].replace(',', ''))
                                    item.get('Damage/Pellet')[2] = float(temp[1].contents[5].replace(',', ''))
                                else:
                                    item.update({'Damage/Pellet': [float(temp[1].contents[0].replace(',', '')) for i in range(0,3)]})
                            else:
                                item.update({temp[0].string.strip(): temp[1].string.strip()})
                        elif (type(temp[0].span) is not type(None) and temp[0].span.contents[0].strip() == 'Pellets/shot'):
                            item.update({'Pellets/shot': temp[1].string.strip()})

                entry = [i[1] for i in item.items()]
                entry = entry[:-1]
                writeToCSV(entry, item.get('Weapon Class'))
                print('Import Succcessful')
            except Exception as e:
                print(e)
                with open('log.txt', 'a') as log:
                    log.write('\n' + weapon_name)

def initCSV():
    os.chdir('../C')
    if not os.path.exists('data'):
        os.makedirs('data')
    os.chdir('data')

    for key in weapon_dictionary.keys():
        with open(key + '.csv', 'w') as csvFile:
            writer = csv.writer(csvFile)
            writer.writerow(weapon_csv_header)
        csvFile.close()
    with open('log.txt', 'w') as log:
        log.write('Weapons that failed to import:')
    log.close()

def writeToCSV(entry, category):
    csvName = category.replace(' ', '_').lower()
    with open(csvName + 's' + '.csv', 'a') as csvFile:
        writer = csv.writer(csvFile, delimiter = ';')
        writer.writerow(entry)
    csvFile.close()

def getSoup(item):
    url = urllib.request.urlopen(baseURL + item)
    soup = BeautifulSoup(url, 'html.parser')
    return soup

if (__name__ == "__main__"):
    main()