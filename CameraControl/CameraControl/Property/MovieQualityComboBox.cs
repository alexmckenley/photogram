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
    class MovieQualityComboBox : PropertyComboBox, IObserver
    {
        private ActionSource _actionSource;

        private EDSDKLib.EDSDK.EdsPropertyDesc _desc;

        public void SetActionSource(ref ActionSource actionSource) { _actionSource = actionSource; }

        public MovieQualityComboBox()
        {
            map.Add(0x00000200, "1920x1080_2398");
            map.Add(0x00000210, "1920x1080_2398_alli_editing");
            map.Add(0x00000230, "1920x1080_2398_ipb_standard");
            map.Add(0x00000310, "1920x1080_2400_alli_editing");
            map.Add(0x00000330, "1920x1080_2400_ipb_standard");
            map.Add(0x00000400, "1920x1080_2500");
            map.Add(0x00000410, "1920x1080_2500_alli_editing");
            map.Add(0x00000430, "1920x1080_2500_ipb_standard");
            map.Add(0x00000500, "1920x1080_2997");
            map.Add(0x00000510, "1920x1080_2997_alli_editing");
            map.Add(0x00000530, "1920x1080_2997_ipb_standard");
            map.Add(0x00000610, "1920x1080_5000_alli_editing");
            map.Add(0x00000630, "1920x1080_5000_ipb_standard");
            map.Add(0x00000710, "1920x1080_5994_alli_editing");
            map.Add(0x00000730, "1920x1080_5994_ipb_standard");
            map.Add(0x00001210, "1920x1080_2398_alli_editing");
            map.Add(0x00001230, "1920x1080_2398_ipb_standard");
            map.Add(0x00001310, "1920x1080_2400_alli_editing");
            map.Add(0x00001330, "1920x1080_2400_ipb_standard");
            map.Add(0x00001410, "1920x1080_2500_alli_editing");
            map.Add(0x00001430, "1920x1080_2500_ipb_standard");
            map.Add(0x00001431, "1920x1080_2500_ipb_light");
            map.Add(0x00001510, "1920x1080_2997_alli_editing");
            map.Add(0x00001530, "1920x1080_2997_ipb_standard");
            map.Add(0x00001531, "1920x1080_2997_ipb_light");
            map.Add(0x00001610, "1920x1080_5000_alli_editing");
            map.Add(0x00001630, "1920x1080_5000_ipb_standard");
            map.Add(0x00001710, "1920x1080_5994_alli_editing");
            map.Add(0x00001730, "1920x1080_5994_ipb_standard");
            map.Add(0x00010600, "1280x720_5000");
            map.Add(0x00010700, "1280x720_5994");
            map.Add(0x00010810, "1280x720_1000_alli_editing");
            map.Add(0x00010910, "1280x720_1199_alli_editing");
            map.Add(0x00011430, "1280x720_2500_ipb_standard");
            map.Add(0x00011431, "1280x720_5000_ipb_standard");
            map.Add(0x00011530, "1280x720_2997_ipb_standard");
            map.Add(0x00011531, "1280x720_2997_ipb_light");
            map.Add(0x00011610, "1280x720_5000_alli_editing");
            map.Add(0x00011630, "1280x720_5000_ipb_standard");
            map.Add(0x00011710, "1280x720_5994_alli_editing");
            map.Add(0x00011730, "1280x720_5994_ipb_standard");
            map.Add(0x00011810, "1280x720_10000_alli_editing");
            map.Add(0x00011830, "1280x720_10000_ipb_standard");
            map.Add(0x00011910, "1280x720_11988_alli_editing");
            map.Add(0x00011930, "1280x720_11988_ipb_standard");
            map.Add(0x00020400, "640x480_2500");
            map.Add(0x00020500, "640x480_2397");
            map.Add(0x00030240, "4096x2160_2398_motion_jpg");
            map.Add(0x00030340, "4096x2160_2400_motion_jpg");
            map.Add(0x00030440, "4096x2160_2500_motion_jpg");
            map.Add(0x00030540, "4096x2160_2997_motion_jpg");
            map.Add(0x00051210, "3840x2160_2398_alli_editing");
            map.Add(0x00051230, "3840x2160_2398_ipb_standard");
            map.Add(0x00051310, "3840x2160_2400_alli_editing");
            map.Add(0x00051330, "3840x2160_2400_ipb_standard");
            map.Add(0x00051410, "3840x2160_2500_alli_editing");
            map.Add(0x00051430, "3840x2160_2500_ipb_standard");
            map.Add(0x00051510, "3840x2160_2997_alli_editing");
            map.Add(0x00051530, "3840x2160_2997_ipb_standard");
            map.Add(0x00031210, "4096x2160_2398_alli_editing");
            map.Add(0x00031230, "4096x2160_2398_ipb_standard");
            map.Add(0x00031310, "4096x2160_2400_alli_editing");
            map.Add(0x00031330, "4096x2160_2400_ipb_standard");
            map.Add(0x00031410, "4096x2160_2500_alli_editing");
            map.Add(0x00031430, "4096x2160_2500_ipb_standard");
            map.Add(0x00031510, "4096x2160_2997_alli_editing");
            map.Add(0x00031530, "4096x2160_2997_ipb_standard");
            map.Add(0x00031610, "4096x2160_5000_alli_editing");
            map.Add(0x00031630, "4096x2160_5000_ipb_standard");
            map.Add(0x00031710, "4096x2160_5994_alli_editing");
            map.Add(0x00031730, "4096x2160_5994_ipb_standard");
            map.Add(0x00051610, "3840x2160_5000_alli_editing");
            map.Add(0x00051630, "3840x2160_5000_ipb_standard");
            map.Add(0x00051710, "3840x2160_5994_alli_editing");
            map.Add(0x00051730, "3840x2160_5994_ipb_standard");
            map.Add(0x000a3270, "5472x2886_2398_raw_standard");
            map.Add(0x000a3370, "5472x2886_2400_raw_standard");
            map.Add(0x000a3470, "5472x2886_2500_raw_standard");
            map.Add(0x000a3570, "5472x2886_2997_raw_standard");
            map.Add(0x000a3670, "5472x2886_5000_raw_standard");
            map.Add(0x000a3770, "5472x2886_5994_raw_standard");
            map.Add(0x08031210, "4096x2160_2398_alli_editing_crop");
            map.Add(0x08031230, "4096x2160_2398_ipb_standard_crop");
            map.Add(0x08031310, "4096x2160_2400_alli_editing_crop");
            map.Add(0x08031330, "4096x2160_2400_ipb_standard_crop");
            map.Add(0x08031410, "4096x2160_2500_alli_editing_crop");
            map.Add(0x08031430, "4096x2160_2500_ipb_standard_crop");
            map.Add(0x08031510, "4096x2160_2997_alli_editing_crop");
            map.Add(0x08031530, "4096x2160_2997_ipb_standard_crop");
            map.Add(0x08031610, "4096x2160_5000_alli_editing_crop");
            map.Add(0x08031630, "4096x2160_5000_ipb_standard_crop");
            map.Add(0x08031710, "4096x2160_5994_alli_editing_crop");
            map.Add(0x08031730, "4096x2160_5994_ipb_standard_crop");
            map.Add(0x00001810, "1920x1080_10000_alli_editing");
            map.Add(0x00001910, "1920x1080_11988_alli_editing");
            map.Add(0xffffffff, "unknown");
        }

        protected override void OnSelectionChangeCommitted(EventArgs e)
        {
            if (this.SelectedItem != null)
            {
                uint key = (uint)_desc.PropDesc[this.SelectedIndex];

                _actionSource.FireEvent(ActionEvent.Command.SET_MOVIEQUALITY, (IntPtr)key);
            }
        }

        public void Update(Observable from, CameraEvent e)
        {

            CameraModel model = (CameraModel)from;
            CameraEvent.Type eventType = CameraEvent.Type.NONE;

            if ((eventType = e.GetEventType()) == CameraEvent.Type.PROPERTY_CHANGED || eventType == CameraEvent.Type.PROPERTY_DESC_CHANGED)
            {
                uint propertyID = (uint)e.GetArg();

                if (propertyID == EDSDKLib.EDSDK.PropID_MovieParam)
                {
                    uint property = model.MovieQuality;

                    //Update property
                    switch (eventType)
                    {
                        case CameraEvent.Type.PROPERTY_CHANGED:
                            this.UpdateProperty(property);
                            break;

                        case CameraEvent.Type.PROPERTY_DESC_CHANGED:
                            _desc = model.MovieQualityDesc;
                            // Ignore PropertyDesc when shooting still images.
                            if (model.FixedMovie == 0)
                            {
                                _desc.NumElements = 0;
                            }
                            this.UpdatePropertyDesc(ref _desc);
                            this.UpdateProperty(property);
                            break;
                    }
                }
            }
        }
    }
}
