from pymongo import MongoClient
from bson.objectid import ObjectId

#initializes MongoClient
client = MongoClient('mongodb://localhost:51733') 


class AnimalShelter(object):
    
    def __init__(self, username = None, password = None):
        #accesses MongoDB databases and collections
        if username and password:
            self.client = MongoClient('mongo://%s:%s@localhost:51733' % (username, password))
            
        else: 
            self.client = MongoClient('mongodb://localhost:51733')
            self.database = self.client['AAC']

    #Implements C in CRUD
    def create(self, data):
        if data is not None:
            insert = self.database.animals.insert(data)
            
            if insert != 0:
                return True
            else:
                return False
        else: raise Exception("Nothing to save parameter is empty")
    
    #Implements R in CRUD
    def read(self, criteria = None):
        
        #if criteria is not none, all rows returned which match criteria
        if criteria:
            {"_id" : False}   #omits unique ID
            
            data = self.database.animals.find(criteria, {"_id":False})
         
        #if no criteria all rows return   
        else: 
            data = self.database.animals.find({}, {"_id":False})
            
        return data
    
        #Implements U in CRUD
    def update(self,data, new):
        if data is not None:
            self.database.animals.update_one(data,{'$set': new})
            return True
        else:
            print("Nothing to update, parameter empty")
            return False
        
        #Implements D in CRUD
    def delete(self, data):
        if data is not None:
            self.database.animals.delete_one(data)
            return True
        else:
            print("Nothing to delete, parameter empty")
            return False  