import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { Storage } from '@ionic/storage';

/*
  Generated class for the PservicesProvider provider.

  See https://angular.io/guide/dependency-injection for more info on providers
  and Angular DI.
*/
@Injectable()
export class PservicesProvider {

  locations: any;
  locationsTemp: any;
  allUsers: any;
  allUsersTemp: any;
  onGoingDeliveries: any;
  onGoingDeliveriesTemp: any;
  

  account: any;
  name: string;
  email: string;
  address: string;
  userIdTag: string;
  password: string;

  createBooking: any;

 
  constructor(public storage: Storage) {
    //this.hotels = HOTELS;
    /**
    this.hotels = 
    [
      {
        id:1,
        name: "Pearl continental hardcoded",
        rating: 4,
        image: "image",
        location: {
          lat: -22.906847,
          lon: -43.172896,
        },
        thumb: "assets/img/hotel/thumb/hotel_1.jpg"
      }
    ]
     */
    this.checkifAccount();
    this.getLocationsFromServer();
    this.getUsersFromServer();
    this.getOngoingDeliveriesFromServer();
    
  }

  checkifAccount(){
    
    this.storage.get('userBasicInfo').then((val) => {
        console.log("account value found", val)
      if (val===null){
        //console.log("no account found. Error!!!", val);
        this.account = null;
        //this.setRoot('page-register');
      }
      else{
        this.account = val;
        this.userIdTag = val.userIdTag
        this.address = val.address;
        this.getLocationsFromServer();
        
        this.name = val.name;
        this.email = val.email;
        this.password = val.password;
        
        //console.log("account found", val);
        
        this.createBooking = {
          'userIdTag': this.userIdTag,
          'senderLocation': '',
          'receiverId': '',
        } 
        console.log("initialized")
      }
    });
     
    1;
  }

  getAcountStatus(){
    console.log("curr account status", this.account)
    return this.account
  }
   
  getAddress(){
    return this.address;
  }

  toDateTime(secs) {
    var t = new Date(1970, 0, 1); // Epoch
    t.setSeconds(secs);
    return t;
  }


  getLocationsFromServer(){
    var InitiateGetTransactions = function(callback) // How can I use this callback?
     {
         var request = new XMLHttpRequest();
         request.onreadystatechange = function()
         {
             if (request.readyState == 4 && request.status == 200)
             {
                 callback(request.responseText); // Another callback here
             }
             if (request.readyState == 4 && request.status == 0)
             {
                 //console.log("no response for resturants") // Another callback here
             }
         }; 
         //console.log("sending _this.userIdTag to server", userIdTag)
         request.open("POST", "https://api.anomoz.com/api/swift/post/read_all_locations.php");
         request.send();
     }
     
     var _this = this;
     var frameTransactions = function mycallback(data) {
      _this.locationsTemp = []
       //console.log("locations received from server," , data)
       var dataParsed
       dataParsed = JSON.parse(data);
       if(dataParsed.message=="none"){
         //console.log("no locations")
       }
       else{
         var sampleTrans = dataParsed
           //console.log(sampleTrans)
           for (var i=0; i<sampleTrans.length; i++){
            //check if hotel already
              var a = {
                id: sampleTrans[i].id,
                name: sampleTrans[i].name,
            }
            _this.locationsTemp.push(a)
          	
          }
          //add to local storage
          _this.locations = _this.locationsTemp
       }
     }
     InitiateGetTransactions(frameTransactions); //passing mycallback as a method 
  }

  getOngoingDeliveriesFromServer(){
    var InitiateGetTransactions = function(callback) // How can I use this callback?
     {
         var request = new XMLHttpRequest();
         request.onreadystatechange = function()
         {
             if (request.readyState == 4 && request.status == 200)
             {
                 callback(request.responseText); // Another callback here
             }
             if (request.readyState == 4 && request.status == 0)
             {
                 //console.log("no response for resturants") // Another callback here
             }
         }; 
         //console.log("sending _this.userIdTag to server", userIdTag)
         request.open("POST", "https://api.anomoz.com/api/swift/post/read_all_users.php");
         request.send();
     }
     
     var _this = this;
     var frameTransactions = function mycallback(data) {
      _this.onGoingDeliveriesTemp = []
       //console.log("locations received from server," , data)
       var dataParsed
       dataParsed = JSON.parse(data);
       if(dataParsed.message=="none"){
         //console.log("no locations")
       }
       else{
         var sampleTrans = dataParsed
           //console.log(sampleTrans)
           for (var i=0; i<sampleTrans.length; i++){
            //check if hotel already
              var a = {
                id: sampleTrans[i].id,
                name: sampleTrans[i].name,
            }
            _this.onGoingDeliveriesTemp.push(a)
          	
          }
          //add to local storage
          _this.onGoingDeliveries = _this.onGoingDeliveriesTemp
       }
     }
     InitiateGetTransactions(frameTransactions); //passing mycallback as a method 
  }

  getUsersFromServer(){
    var InitiateGetTransactions = function(callback) // How can I use this callback?
     {
         var request = new XMLHttpRequest();
         request.onreadystatechange = function()
         {
             if (request.readyState == 4 && request.status == 200)
             {
                 callback(request.responseText); // Another callback here
             }
             if (request.readyState == 4 && request.status == 0)
             {
                 //console.log("no response for resturants") // Another callback here
             }
         }; 
         //console.log("sending _this.userIdTag to server", userIdTag)
         request.open("POST", "https://api.anomoz.com/api/swift/post/read_all_users.php");
         request.send();
     }
     
     var _this = this;
     var frameTransactions = function mycallback(data) {
      _this.allUsersTemp = []
       //console.log("locations received from server," , data)
       var dataParsed
       dataParsed = JSON.parse(data);
       if(dataParsed.message=="none"){
         //console.log("no locations")
       }
       else{
         var sampleTrans = dataParsed
           //console.log(sampleTrans)
           for (var i=0; i<sampleTrans.length; i++){
            //check if hotel already
              var a = {
                id: sampleTrans[i].userIdTag,
                name: sampleTrans[i].name,
                email: sampleTrans[i].email
            }
            _this.allUsersTemp.push(a)
          	
          }
          //add to local storage
          _this.allUsers = _this.allUsersTemp
       }
     }
     InitiateGetTransactions(frameTransactions); //passing mycallback as a method 
  }

  getAllLocations() {
    return this.locations;
  }

  getAllUsers(){
    return this.allUsers
  }


  storeSignupData(name, email, password, userIdTag){
    this.name = name;
    this.email = email;
    this.password = password;
    this.userIdTag = userIdTag;

    var tempInfo = {
      "name": this.name,
      "email": this.email,
      "password": this.password,
      "userIdTag": this.userIdTag,
      "address": ''    
    }
    // set a key/value
    //console.log("storage updated to (signup):", tempInfo);
    this.account = tempInfo
    console.log("storing data", tempInfo)
    this.storage.set('userBasicInfo', tempInfo);
  }

  getOnGoingDeliveries(){
    return this.onGoingDeliveries;
  }
  
}
