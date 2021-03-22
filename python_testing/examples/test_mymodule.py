import unittest
from  unittest.mock import patch
from  unittest import mock
from  mymodule import RemovalService
from  mymodule import UploadService
from  mymodule import Calculator


class RemovalServiceTestCase(unittest.TestCase):

    @patch('mymodule.os.path.isfile')
    @patch('mymodule.os.remove')
    def test_rm_file_does_not_exist(self, mymodule_os_mock, mymodule_os_path_isfile_mock):
        reference = RemovalService()
        mymodule_os_path_isfile_mock.return_value = False
        reference.rm('some_file')
        self.assertFalse(mymodule_os_mock.remove.called, "Failed to remove file not present")

    @patch('mymodule.os.path.isfile')
    @patch('mymodule.os.remove')
    def test_rm_file_exist(self, mymodule_os_mock, mymodule_os_path_isfile_mock):
        reference = RemovalService()
        mymodule_os_path_isfile_mock.return_value = True
        reference.rm('some_file')
        mymodule_os_mock.assert_called_with('some_file')


class UploadServiceTestCaseInstanceWithMockedMethod(unittest.TestCase):

    @patch.object(RemovalService, 'rm')  ##mocking instance method 'rm'-> "real" instance with mocked method
    def test_upload_complete_instance_mocked_method(self, mock_rm):

        removalService = RemovalService()
        reference = UploadService(removalService)
        reference.upload_complete("my uploaded file")
        mock_rm.assert_called_with("my uploaded file")
        removalService.rm.assert_called_with("my uploaded file")



class UploadServiceTestCaseMockInstance(unittest.TestCase):

    def test_upload_complete_mock_instance(self):
        mock_removal_service = mock.create_autospec(RemovalService)
        reference = UploadService(mock_removal_service)
        reference.upload_complete("my uploaded file")
        mock_removal_service.rm.assert_called_with("my uploaded file")

## w/o mocking -creating and deleting files
#from mymodule import rm

#import os.path
#import tempfile
#import unittest
#
#class RmTestCase(unittest.TestCase):
#    tmpfilepath = os.path.join(tempfile.gettempdir() , "tmp-testfile")
#
#    def setUp(self):
#        with open(self.tmpfilepath, 'w') as f:
#            f.write("remove me")

#    def test_rm(self):
#        rm(self.tmpfilepath)
#        self.assertFalse(os.path.isfile(self.tmpfilepath), "Failed to remove file")
#


class TestCalculator(unittest.TestCase):
    @patch.object(Calculator, 'sum', return_value=9)
    def test_sum(self, sum):
        self.assertEqual(sum(2,3), 9) ## no wlasnie!! 2+3 = 9 !! Uwazaj...


class TestCalculator(unittest.TestCase):
    def setUp(self):
        self.calc = Calculator()

    def test_sum(self):
        answer = self.calc.sum(2, 3)
        self.assertEqual(answer, 9)

