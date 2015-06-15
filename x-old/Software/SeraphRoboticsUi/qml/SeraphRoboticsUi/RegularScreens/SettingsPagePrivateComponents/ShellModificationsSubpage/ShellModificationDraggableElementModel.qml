import QtQuick 2.4

ListModel {
    id : myModel

    function checkIfNameExist(searchName)
    {
        for (var i = 0; i < myModel.count; i++)
        {
            if(myModel.get(i).name === searchName)
            {
                return true
            }
        }
        return false
    }

    ListElement {
        svgItem : "qrc:/exampleImages/3.svg"
        name : "U-Shaped Pad"
        m_Width : -1
        m_Height : -1
        basicRotation : 0
    }
    ListElement {
        name : "Create Custom Pad"
        m_Width : -1
        m_Height : -1
        basicRotation : 0
    }
    ListElement {
        svgItem : "qrc:/exampleImages/4.svg"
        name : "Heel Pad"
        m_Width : -1
        m_Height : -1
        basicRotation : 0
    }
    ListElement {
        svgItem : "qrc:/exampleImages/5.svg"
        name : "Heel Spur Hole"
        m_Width : -1
        m_Height : -1
        basicRotation : 0
    }
    ListElement {
        svgItem : "qrc:/exampleImages/6.svg"
        name : "Met Pad"
        m_Width : -1
        m_Height : -1
        basicRotation : 0
    }
    ListElement {
        svgItem : "qrc:/exampleImages/7.svg"
        name : "Met Bar Pad"
        m_Width : -1
        m_Height : -1
        basicRotation : 0
    }
    ListElement {
        svgItem : "qrc:/exampleImages/8.svg"
        name : "Dancer's Pad"
        m_Width : -1
        m_Height : -1
        basicRotation : 0
    }
    ListElement {
        svgItem : "qrc:/exampleImages/9.svg"
        name : "Morton's Extension"
        m_Width : -1
        m_Height : -1
        basicRotation : 0
    }

    ListElement {
        svgItem : "qrc:/exampleImages/10.svg"
        name : "Neuroma Pad"
        m_Width : -1
        m_Height : -1
        basicRotation : 0
    }
    ListElement {
        svgItem : "qrc:/exampleImages/11.svg"
        name : "Scaphoid Pad"
        m_Width : -1
        m_Height : -1
        basicRotation : 0
    }
}
