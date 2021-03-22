import os
import time

class RemovalService(object):

    def rm(self, filename):
        if os.path.isfile(filename):
            os.remove(filename)


class UploadService(object):
    def __init__(self, removal_service):
        self.removal_service = removal_service

    def upload_complete(self, filename):
        self.removal_service.rm(filename)



class Calculator(object):
    def sum(self, a, b):
        time.sleep(1)
        return a + b
