from PyQt5.QtCore import *

import re, ast

class WatcherSignals(QObject):
    update = pyqtSignal(dict)
    update_map = pyqtSignal(list)
    update_cur = pyqtSignal(tuple)
    update_dest = pyqtSignal(tuple)
    update_done = pyqtSignal()
    update_radar = pyqtSignal(tuple)

class Watcher(QObject):
    def endParse(self, message):
        message = message.rstrip()
        if self.rate_match.search(message):
            pos = self.rate_match.search(message)
            self.rate = int(message[pos.end()+1:])
        elif self.suc_match.search(message):
            self.status = True
            self.err_type = -1
        elif self.err_match.search(message):
            self.status = False
            if self.seq_match.search(message) != None:
                self.err_type = 0
                self.num_pic_seq_err = self.num_pic_seq_err + 1
            elif self.che_match.search(message) != None:
                self.err_type = 1
                self.num_pic_checksum_err = self.num_pic_checksum_err + 1
            else:
                self.err_type = -1
                self.num_pic_unknown_err = self.num_pic_unknown_err + 1
        elif self.inv_match.search(message):
            #check which
            self.status = False
            if self.che_match.search(message):
                self.err_type = 3
                self.num_server_checksum_err = self.num_server_checksum_err + 1
            elif self.seq_match.search(message):
                self.err_type = 2
                self.num_server_seq_err = self.num_server_seq_err + 1
            elif self.cmd_match.search(message):
                self.err_type = 4
                self.num_server_cmd_err = self.num_server_cmd_err + 1
        elif self.v_match.search(message):
            self.status = True
            self.err_type = -1
        elif self.map_match.search(message):
            self.map = ast.literal_eval(message.split(': ')[1])
            return 0
        elif self.cur_match.search(message):
            self.cur_pos = ast.literal_eval(message.split(': ')[1])
            return 2
        elif self.move_match.search(message):
            res = self.move_match.search(message)
            self.cur_pos = ast.literal_eval(message[res.start()+9:res.end()])
            return 2
        elif self.dest_match.search(message):
            self.dest = ast.literal_eval(message.split(': ')[1])
            return 3
        elif self.done_match.search(message):
            return 4
        elif self.radar_match.search(message):
            split = message.split(',')
            self.tick = int(split[0].split(':')[1])
            self.tick_value = int(split[1].split(':')[1])
            return 5
        return 1

    def __init__(self):
        super(Watcher,self).__init__()
        #regexes
        self.suc_match = re.compile('SUCCESS')
        self.err_match = re.compile('Error')
        self.seq_match = re.compile('seq number')
        self.che_match = re.compile('checksum')
        self.rate_match = re.compile('Rate.*:')
        self.inv_match = re.compile('invalid')
        self.cmd_match = re.compile('cmd')
        self.v_match = re.compile('valid')
        self.map_match = re.compile('MAP_')
        self.cur_match = re.compile('Cur Pos:')
        self.move_match = re.compile('moved to.*\)')
        self.dest_match = re.compile('Dest:')
        self.done_match = re.compile('DONE')
        self.radar_match = re.compile('tick:')
        #signal
        self.signals = WatcherSignals()
        # data
        self.status = True
        self.err_type = -1
        self.rate = 0
        self.num_server_seq_err = 0
        self.num_server_checksum_err = 0
        self.num_pic_unknown_err = 0
        self.num_pic_checksum_err = 0
        self.num_pic_seq_err = 0
        self.num_server_cmd_err = 0
        self.map = []
        self.cur_pos = (0,0)
        self.dest = (0,0)
        self.tick = 0
        self.tick_value = 0

    def start_parse(self, message):
        res = self.endParse(message)
        if res == 0:
            self.signals.update_map.emit(self.map)
        elif res == 2:
            self.signals.update_cur.emit(self.cur_pos)
        elif res == 3:
            self.signals.update_dest.emit(self.dest)
        elif res == 4:
            self.signals.update_done.emit()
        elif res == 5:
            self.signals.update_radar.emit((self.tick,self.tick_value))
        elif res == 1:
            dataToSend = {
                    'status':self.status,
                    'err_type':self.err_type,
                    'rate':self.rate,
                    'num_server_seq_err': self.num_server_seq_err,
                    'num_server_checksum_err': self.num_server_checksum_err,
                    'num_pic_unknown_err': self.num_pic_unknown_err,
                    'num_pic_checksum_err': self.num_pic_checksum_err,
                    'num_pic_seq_err': self.num_pic_seq_err,
                    'num_server_cmd_err':self.num_server_cmd_err
                    }
            self.signals.update.emit( dataToSend )

