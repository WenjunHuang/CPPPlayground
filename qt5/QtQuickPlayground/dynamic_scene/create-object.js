class ObjectBuilder {
    constructor(source, parent, callback) {
        this.source = source
        this.parent = parent
        this.callback = callback
    }

    create() {
        const component = Qt.createComponent(this.source)
        this.component = component
        if (component.status === Component.Ready || component.status === Component.Error)
            this.createDone()
        else
            component.statusChanged.connect(this.createDone)
    }

    createDone(){
        if (this.component.status === Component.Ready) {
            const obj = this.component.createObject(parent)
            if (obj != null) {
                this.callback(obj,this.source)
            } else {
                console.log("Error creating object: " + this.component.errorString())
            }
            this.component.destroy()

        } else if (this.component.status === Component.Error) {
                console.log("Error creating object: " + this.component.errorString())
        }
    }
}

