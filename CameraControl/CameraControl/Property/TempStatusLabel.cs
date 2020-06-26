/******************************************************************************
*                                                                             *
*   PROJECT : Eos Digital camera Software Development Kit EDSDK               *
*                                                                             *
*   Description: This is the Sample code to show the usage of EDSDK.          *
*                                                                             *
*                                                                             *
*******************************************************************************
*                                                                             *
*   Written and developed by Canon Inc.                                       *
*   Copyright Canon Inc. 2018 All Rights Reserved                             *
*                                                                             *
*******************************************************************************/

using System;

namespace CameraControl
{
    class TempStatusLabel : InfoLabel, IObserver
    {

        public void Update(Observable from, CameraEvent e)
        {

            CameraEvent.Type eventType = CameraEvent.Type.NONE;

            if ((eventType = e.GetEventType()) == CameraEvent.Type.PROPERTY_CHANGED)
            {
                uint propertyID = (uint)e.GetArg();

                if (propertyID == EDSDKLib.EDSDK.PropID_TempStatus)
                {
                    //Update property
                    switch (eventType)
                    {
                        case CameraEvent.Type.PROPERTY_CHANGED:
                            CameraModel model = (CameraModel)from;
                            var infoText = new string[] { "Normal", "Warning", "Frameratedown", "Disableliveview", "Disablerelease", "Stillqualitywarning", "unknown" };
                            if (infoText.Length > model.TempStatus)
                            {
                                this.UpdateProperty(infoText[model.TempStatus]);
                            }
                            else
                            {
                                this.UpdateProperty(infoText[infoText.Length - 1]);
                            }
                            break;
                    }
                }
            }
        }
    }
}
