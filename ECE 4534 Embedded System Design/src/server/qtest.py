from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5.QtChart import *

import time, sys, errno
import socket
import pickle

from watcher import Watcher

from client_debug import UserClient

class WorkerSignals(QObject):
    update = pyqtSignal(dict)
    update_map = pyqtSignal(list)
    update_cur = pyqtSignal(tuple)
    update_radar = pyqtSignal(tuple)
    update_dest = pyqtSignal(tuple)
    update_done = pyqtSignal()

class Worker(QRunnable):
    def __init__(self, host, port, *args, **kwargs):
        super(Worker, self).__init__()
        self.signals = WorkerSignals()
        self.killed = False
        self.port = port
        self.host = host

    def killme(self):
        self.killed = True

    def isalive(self):
        return self.killed == False

    @pyqtSlot()
    def run(self):
        handler = Watcher()
        handler.signals.update.connect(self.signals.update)
        handler.signals.update_map.connect(self.signals.update_map)
        handler.signals.update_cur.connect(self.signals.update_cur)
        handler.signals.update_dest.connect(self.signals.update_dest)
        handler.signals.update_done.connect(self.signals.update_done)
        handler.signals.update_radar.connect(self.signals.update_radar)
        recvSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        recvSocket.bind((self.host, self.port))
        recvSocket.setblocking(False)
        while self.isalive():
            try:
                data = recvSocket.recv(4096)
            except socket.timeout as e:
                err = e.args[0]
                if err == 'timed out':
                    continue
                else:
                # a "real" error occurred
                    print(e)
                    sys.exit(1)
            except socket.error as e:
                err = e.args[0]
                if err == errno.EAGAIN or err == errno.EWOULDBLOCK:
                    continue
                else:
                # a "real" error occurred
                    print(e)
                    sys.exit(1)
            else:
                message = pickle.loads( data )
                print(message)
                handler.start_parse(message)

class DriveSignals(QObject):
    keyPressed = pyqtSignal(QEvent)

class ManualDrive(QDialog):
    def __init__(self):
        super(ManualDrive, self).__init__()
        self.setGeometry(300,300,250,150)
        self.signals = DriveSignals()

    def keyPressEvent(self, event):
        super(ManualDrive, self).__init__()
        self.signals.keyPressed.emit(event)

class TabContent(QTabWidget):
    def __init__(self, port, client, *args, **kwargs):
        super(QTabWidget, self).__init__(*args, **kwargs)
        self.client = client
        self.port = port
        self.cur_pos = (0,0)
        self.dest = (0,0)
        self.mappingTestActive = False
        self.mappingTestStepNum = 0
        self.manualDriveWindow = ManualDrive()

        self.manualDriveWindow.signals.keyPressed.connect(self.manualKey)

        mainLayout = QGridLayout()

        #stats window
        statsG = QGroupBox('Statistics')
        stats = QHBoxLayout()
        self.stats = QLabel('Stats')
        stats.addWidget(self.stats)

        #Manual Control window
        manualControlG = QGroupBox('Manual Control')
        manualControl = QGridLayout()
        self.sendForwardButton = QPushButton('Forward')
        self.sendLeftButton = QPushButton('Left')
        self.sendRightButton = QPushButton('Right')
        self.sendBackwardButton = QPushButton('Backward')
        manualControl.addWidget(self.sendForwardButton, 0, 1)
        manualControl.addWidget(self.sendLeftButton, 1, 0)
        manualControl.addWidget(self.sendRightButton, 1, 2)
        manualControl.addWidget(self.sendBackwardButton, 2, 1)
        manualControlG.setLayout(manualControl)

        stats.addWidget(manualControlG)
        statsG.setLayout(stats)

        #map
        mmapG = QGroupBox('Map')
        self.mmap = QGridLayout()
        for y in range(10):
            for x in range(10):
                cell = QLabel('{}'.format(y*10+x))
                cell.setToolTip('({},{}): {}'.format(x,y,y*10+x))
                self.mmap.addWidget(cell,y,x)

        self.updateCur(self.cur_pos)
        self.updateDest(self.dest)

        mmapG.setLayout(self.mmap)

        #command Group
        commandG = QGroupBox('Commands')
        commandLayout = QFormLayout()

        #send start pos ( 2 input boxes and a button
        sendStartPos = QHBoxLayout()
        self.sendStartPosX = QLineEdit('X')
        self.sendStartPosY = QLineEdit('Y')
        self.sendStartPosButton = QPushButton('Send Start Pos')
        sendStartPos.addWidget(self.sendStartPosX)
        sendStartPos.addWidget(self.sendStartPosY)
        sendStartPos.addWidget(self.sendStartPosButton)
        commandLayout.addRow(sendStartPos)

        #send dest
        sendDestPos = QHBoxLayout()
        self.sendDestPosX = QLineEdit('X')
        self.sendDestPosY = QLineEdit('Y')
        self.sendDestPosButton = QPushButton('Send DestPos')
        sendDestPos.addWidget(self.sendDestPosX)
        sendDestPos.addWidget(self.sendDestPosY)
        sendDestPos.addWidget(self.sendDestPosButton)
        commandLayout.addRow(sendDestPos)

        #start stop buttons
        sendStartStop = QHBoxLayout()
        self.sendStartButton = QPushButton('Start Nav')
        self.sendStopButton = QPushButton('Stop Nav')
        sendStartStop.addWidget(self.sendStartButton)
        sendStartStop.addWidget(self.sendStopButton)
        commandLayout.addRow(sendStartStop)

        #arbitrary command
        sendCommand = QHBoxLayout()
        self.sendCommandCmd = QLineEdit('cmd')
        self.sendCommandType = QLineEdit('type')
        self.sendCommandData = QLineEdit('data')
        self.sendCommandButton = QPushButton('Send Command')
        sendCommand.addWidget(self.sendCommandCmd)
        sendCommand.addWidget(self.sendCommandType)
        sendCommand.addWidget(self.sendCommandData)
        sendCommand.addWidget(self.sendCommandButton)
        commandLayout.addRow(sendCommand)

        #mapping test
        mappingCommand = QHBoxLayout()
        self.startMappingTestButton = QPushButton('Start/Stop Scanning')
        mappingCommand.addWidget(self.startMappingTestButton)
        commandLayout.addRow(mappingCommand)

        #refresh map
        refMap = QHBoxLayout()
        self.refMapButton = QPushButton('Refresh Map')
        refMap.addWidget(self.refMapButton)
        commandLayout.addRow(refMap)

        #manual corrections
        manC = QHBoxLayout()
        self.refManualCorrections = QPushButton('Manual Corrections')
        manC.addWidget(self.refManualCorrections)
        commandLayout.addRow(manC)

        commandG.setLayout(commandLayout)

        #radar view
        radarG = QGroupBox('RADAR')
        radarL = QHBoxLayout()
        radarV = QChartView()
        #create chart
        radarChart = QPolarChart()
        #create data series
        self.radarData = QLineSeries()
        for i in range(513):
            self.radarData.append(i*(360.0/513.0),i*(380/513))
        radarChart.addSeries(self.radarData)
        #create angular axis
        radarValueAxis = QValueAxis()
        radarValueAxis.setRange(0,512)
        radarValueAxis.setTickCount(5)
        radarValueAxis.setLabelFormat('%d')
        radarChart.addAxis(radarValueAxis, QPolarChart.PolarOrientationAngular)
        #create radial axis
        radarRadialAxis = QValueAxis()
        radarRadialAxis.setTickCount(9)
        radarRadialAxis.setRange(4,400)
        radarRadialAxis.setLabelFormat('%d')
        radarChart.addAxis(radarRadialAxis, QPolarChart.PolarOrientationRadial)
        #add everything to the chart
        self.radarData.attachAxis(radarValueAxis)
        self.radarData.attachAxis(radarRadialAxis)
        radarV.setChart(radarChart)
        radarL.addWidget(radarV);

        radarG.setLayout(radarL)

        #add to main layout
        mainLayout.addWidget(statsG,0,0)
        mainLayout.addWidget(mmapG,1,0)
        mainLayout.addWidget(commandG,0,1)
        mainLayout.addWidget(radarG,1,1)

        mainLayout.setColumnStretch(1, 1)
        mainLayout.setColumnStretch(0, 2)

        self.setLayout(mainLayout)

        #connect buttons
        self.sendForwardButton.clicked.connect(self.handleSendForwardButtonClick)
        self.sendLeftButton.clicked.connect(self.handleSendLeftButtonClick)
        self.sendRightButton.clicked.connect(self.handleSendRightButtonClick)
        self.sendBackwardButton.clicked.connect(self.handleSendBackwardButtonClick)
        self.sendCommandButton.clicked.connect(self.handleSendCommandButtonClick)
        self.sendStartPosButton.clicked.connect(self.handleSendStartPosButtonClick)
        self.sendDestPosButton.clicked.connect(self.handleSendDestPosButtonClick)
        self.sendStartButton.clicked.connect(self.handleSendStartButtonClick)
        self.sendStopButton.clicked.connect(self.handleSendStopButtonClick)
        self.startMappingTestButton.clicked.connect(self.handleStartMappingTestButton)
        self.refMapButton.clicked.connect(self.handleRefMapButton)
        self.refManualCorrections.clicked.connect(self.handleManualCorrectionsOpen)

    def handleSendForwardButtonClick(self):
        cmd = 0x30
        typ = 100
        data = 1
        self.client.exec(cmd,typ,data,self.port)

    def handleSendBackwardButtonClick(self):
        cmd = 0x31
        typ = 100
        data = 1
        self.client.exec(cmd,typ,data,self.port)

    def handleSendLeftButtonClick(self):
        cmd = 0x32
        typ = 100
        data = 1
        self.client.exec(cmd,typ,data,self.port)

    def handleSendRightButtonClick(self):
        cmd = 0x33
        typ = 100
        data = 1
        self.client.exec(cmd,typ,data,self.port)

    def handleManualCorrectionsOpen(self):
        self.manualDriveWindow.show()
    
    def handleSendCommandButtonClick(self):
        cmd = int(self.sendCommandCmd.text(),16)
        typ = int(self.sendCommandType.text())
        data = int(self.sendCommandData.text())
        self.client.exec(cmd,typ,data,self.port)

    def handleSendStartPosButtonClick(self):
        cmd = 0x19
        typ = int(self.sendStartPosX.text())
        data = int(self.sendStartPosY.text())
        self.client.exec(cmd,typ,data,self.port)

    def handleSendDestPosButtonClick(self):
        cmd = 0x18
        typ = int(self.sendDestPosX.text())
        data = int(self.sendDestPosY.text())
        self.client.exec(cmd,typ,data,self.port)

    def handleSendStartButtonClick(self):
        cmd = 0x17
        typ = 0
        data = 0
        self.client.exec(cmd,typ,data,self.port)

    def handleSendStopButtonClick(self):
        cmd = 0x1e
        typ = 0
        data = 0
        self.client.exec(cmd,typ,data,self.port)

    def handleStartMappingTestButton(self):
        cmd = 0x2a
        typ = 0
        data = 0
        self.client.exec(cmd,typ,data,self.port)

    def handleRefMapButton(self):
        cmd = 0x25
        typ = 0
        data = 0
        self.client.exec(cmd,typ,data,self.port)

    #slots
    def manualKey(self, event):
        print('key pressed handler')
        if event.key() == Qt.Key_Left:
            print('l')
            self.client.exec(0x32,75,1,self.port)
        elif event.key() == Qt.Key_Right:
            print('r')
            self.client.exec(0x33,75,1,self.port)
        elif event.key() == Qt.Key_Up:
            print('u')
            self.client.exec(0x30,75,1,self.port)
        elif event.key() == Qt.Key_Down:
            print('d')
            self.client.exec(0x31,75,1,self.port)

    def updateStats(self, stats):
        text = ''
        for key in stats:
            text += '{} : {}\n'.format(key, stats[key])
        self.stats.setText(text)

    def updateMap(self, mapArray):
        for y in range(10):
            for x in range(10):
                widg = self.mmap.itemAtPosition(y,x).widget()
                value = mapArray[y*10+x]
                widg.setText('{}'.format(value))
                if value is 1:
                    widg.setStyleSheet('QLabel {background-color: black;}')
                elif value is 2:
                    widg.setStyleSheet('QLabel {background-color: red;}')
                else:
                    widg.setStyleSheet('QLabel {background-color: white;}')

    def updateCur(self, pos):
        #take away old
        self.mmap.itemAtPosition(self.cur_pos[1],self.cur_pos[0]).widget().setStyleSheet('QLabel {background-color: white;}')
        self.cur_pos = pos
        self.mmap.itemAtPosition(pos[1],pos[0]).widget().setStyleSheet('QLabel {background-color: green;}')

    def updateDest(self, pos):
        #take away old
        self.mmap.itemAtPosition(self.dest[1],self.dest[0]).widget().setStyleSheet('QLabel {background-color: white;}')
        self.dest = pos
        self.mmap.itemAtPosition(pos[1],pos[0]).widget().setStyleSheet('QLabel {background-color: yellow;}')

    def updateDone(self):
        if self.mappingTestActive:
            self.mappingTestExecStep()

    def updateRadar(self, tick):
        try:
            self.radarData.replace(513-tick[0],513-tick[0],tick[1])
        except:
            print("ERROR REPLACING")

class MainWindow(QMainWindow):
    #signals
    killworker = pyqtSignal()

    def __init__(self, ip, port, *args, **kwargs):
        super(MainWindow, self).__init__(*args, **kwargs)

        self.userClient = UserClient(ip,port)

        self.content = QTabWidget()

        self.tab10000 = TabContent(10000,self.userClient)
        self.tab20000 = TabContent(20000,self.userClient)
        self.tab30000 = TabContent(30000,self.userClient)

        self.content.addTab(self.tab10000,'10000')
        self.content.addTab(self.tab20000,'20000')
        self.content.addTab(self.tab30000,'30000')

        self.setCentralWidget(self.content)

        self.setMinimumSize(800,575)

        self.show()

        self.threadpool = QThreadPool()

        self.worker1 = Worker(ip, 10001)
        self.worker2 = Worker(ip, 20001)
        self.worker3 = Worker(ip, 30001)

        self.worker1.signals.update.connect(self.tab10000.updateStats)
        self.worker1.signals.update_map.connect(self.tab10000.updateMap)
        self.worker1.signals.update_cur.connect(self.tab10000.updateCur)
        self.worker1.signals.update_dest.connect(self.tab10000.updateDest)
        self.worker1.signals.update_done.connect(self.tab10000.updateDone)
        self.worker1.signals.update_radar.connect(self.tab10000.updateRadar)
        
        self.worker2.signals.update.connect(self.tab20000.updateStats)
        self.worker2.signals.update_map.connect(self.tab20000.updateMap)
        self.worker2.signals.update_cur.connect(self.tab20000.updateCur)
        self.worker2.signals.update_dest.connect(self.tab20000.updateDest)
        self.worker2.signals.update_done.connect(self.tab20000.updateDone)
        self.worker2.signals.update_radar.connect(self.tab20000.updateRadar)

        self.worker3.signals.update.connect(self.tab30000.updateStats)
        self.worker3.signals.update_map.connect(self.tab30000.updateMap)
        self.worker3.signals.update_cur.connect(self.tab30000.updateCur)
        self.worker3.signals.update_dest.connect(self.tab30000.updateDest)
        self.worker3.signals.update_done.connect(self.tab30000.updateDone)
        self.worker3.signals.update_radar.connect(self.tab30000.updateRadar)

        self.killworker.connect(self.worker1.killme)
        self.killworker.connect(self.worker2.killme)
        self.killworker.connect(self.worker3.killme)

        self.threadpool.start(self.worker1)
        self.threadpool.start(self.worker2)
        self.threadpool.start(self.worker3)

    def closeEvent(self, event):
        print('close')
        self.userClient.close()
        print('emit')
        self.killworker.emit()
        print('wait')
        self.threadpool.waitForDone()

def main():
    if len(sys.argv) != 3:
        exit(1)
    app = QApplication(sys.argv)
    window = MainWindow(sys.argv[1],int(sys.argv[2]))
    exit(app.exec_())

main()
