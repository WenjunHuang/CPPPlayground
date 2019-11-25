import QtQuick 2.12
import People 1.0

BirthdayParty{
    id:root
    property var foo:BirthdayParty.Classic
    property var bar:BirthdayParty.Small
    Component.onCompleted:{
    console.log(food.china)

    console.log(root.fuck.name)
    }
    //foo:"abc"
    host:Boy {
        name:"Bob Jones"
        favor:food["china"]
        shoe{size:12;color:"white";brand:"Bikey";price:90.0}
    }
        Boy{name:"Leo Hodges"
        favor:food.japan
        BirthdayParty.rsvp:"2009-07-01"
            shoe{
                size:8
                color:"blue"
                brand:"Luma"
                price: 1.0
            }
            Component.onCompleted:{
            console.log(shoe.price)
            }
        }
        Girl{name:"Anne Brown"
            shoe.size: 7
            shoe.color:"red"
            shoe.brand:"Job Macobs"
            shoe.price:699.99
            favor:food.china
        BirthdayParty.rsvp:"2009-07-06"
        }
}