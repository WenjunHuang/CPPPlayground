import QtQuick 2.12

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

}
