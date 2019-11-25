import QtQuick 2.12
import Foo 1.0
Item {
    function readValues(anArray,anObject) {
        for (let i = 0;i<anArray.length;i++)
            console.log("Array item:",anArray[i])
        for (let prop in anObject) {
            console.log("Object item:",prop,"=",anObject[prop])
        }
    }

    function readDate(dt) {
        console.log("The given date is:",dt.toUTCString())
        return new Date()
    }

    Component.onCompleted: {
        const data = dataMaker.getData()
        console.log(data instanceof Data)
        console.log(data.alias)
        console.log(`${data.name} has ${data.friends.length} friends:`)
        for (let friend of data.friends){
            const hobbies = friend.hobbies.join()

            console.log(`    ${friend.name} likes ${hobbies}`)
        }
    }

}
