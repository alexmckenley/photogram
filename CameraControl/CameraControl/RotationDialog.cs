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
using System.Windows.Forms;
using System.Security.Permissions;

namespace CameraControl
{

    public partial class RotationDialog : Form, IObserver
    {
        CameraController _controller = null;
        private Label status;
        private Label progressCount;
        private Label label1;
        private Label label2;
        private RotationDialog frmRotationDialog;
        private int photosPerRotation= 5;
        private int currentPhoto = 0;

        private ActionSource _actionSource = null;

        public RotationDialog(ref CameraController controller, ref ActionSource actionSource)
        {
            IObserver iObs = this;

            InitializeComponent();
            frmRotationDialog = this;
            _controller = controller;
            _actionSource = actionSource;

            progressLabel.Text = $"{currentPhoto} / {photosPerRotation}";

            _actionSource.FireEvent(ActionEvent.Command.TAKE_PICTURE, IntPtr.Zero);
        }

        ~RotationDialog()
        {
            //Do not called at the appropiate time.

        }

        public RotationDialog getFormRotationDialog { get { return frmRotationDialog; } }

        public void Update(Observable from, CameraEvent e)
        {
            Update(e);
        }

        private delegate void _Update(CameraEvent e);

        private void Update(CameraEvent e)
        {
            if (this.InvokeRequired)
            {
                //The update processing can be executed from another thread. 
                this.Invoke(new _Update(Update), new object[] { e });
                return;
            }

            CameraEvent.Type eventType = e.GetEventType();

            switch (eventType)
            {
                case CameraEvent.Type.SHUT_DOWN:
                    CameraModel cameramodel = _controller.GetModel();
                    cameramodel._ExecuteStatus = CameraModel.Status.CANCELING;
                    break;
                default:
                    break;
            }
        }

        private void FormRotationDialog_FormClosing(object sender, FormClosingEventArgs e)
        {
            CameraModel cameramodel = _controller.GetModel();
            cameramodel._ExecuteStatus = CameraModel.Status.CANCELING;
        }
    }

}
