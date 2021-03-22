##inne ciekawe przyklady prostego skryptu 
# see crashbacktracer.py





import pytest
from libraries.bts_operations import BtsSmSession, bts_operations
from mock import call, patch



@patch('libraries.ta_btssm_operations.ta_btssm_operations')
def test_infomodel_session_called_with_default_alias(ta_moc, expected_calls_for_default_alias):
    with BtsSmSession():
        pass
    assert ta_moc.mock_calls == expected_calls_for_default_alias


##Jak to działa - @patch
# patch to jest dekorator ktory tworzy mocka ktory pozniej jest dostepny w tescie 



#tutaj    libraries.ta_btssm_operations.ta_btssm_operations
# jest dostepne w tescie jako  bts_operation_mock 
# w specyficznych przypadkach trzeba jeszcze zrobic instancje mocka 
# specyficzny przypadek jest wtedy gdy mockiem jest ala biblioteka np.


@patch('libraries.ta_btssm_operations.ta_btssm_operations')
def test_reset_enb_calls_for_testline_management(bts_operation_mock):
    bts_operation_mock_instance = bts_operation_mock.return_value     #(1) tworzymy iunstancje mocka
    bta = bts_operations()                                     #(2) tworzymy instancje testowanej klasy 
    bta.reset_enb()                                            #3 wolamy metode ktora testujemy - testujemy czyli sprawdzamy 
                                                               # czy zawolaly sie odpowiednie metody innych klas

    bts_operation_mock_instance.reset_bts.assert_called_once()        #sprawdzamy czy sie zawowaly odpowiednie metody 

------------------------------------
##jak to wyglada w kodzie ?
###testujemy klase ktora ma metode reset_enb     Oto ona :


class bts_operations(object):
    ROBOT_LIBRARY_SCOPE = 'TEST CASE'

    def __init__(self):
        pass

    def reset_enb(self):
        with BtsSmSession() as btssm:
           btssm.reset_bts()

#metoda reset_bts tworzy instancje klasy  BtsSmSession i na niej wola  btssm.reset_bts()
#zobaczny teraz jak wyglada klasa BtsSmSession


class BtsSmSession(Session):
    def __init__(self, alias="default"):
        Session.__init__(self, 'ta_btssm_operations', 'setup_btssm', 'teardown_btssm', alias=alias)

#ta klasa nie ma metody  reset_bts ... a przeciez to ja testujemy 
# co robi init tej klasy ??

#wola konstruktor klasy Session w parametrach podajac mu ta_btssm_operations -- nazwa biblioteki
#metode dla __init__ : setup_btssm
#metode dla exit : teardown_btssm
 
#te metody potrzebne sa w momencie gdy wykorzystujemy obiekt klasy session / BtsSession w konstrukcji z with 

    with BtsSmSession(alias='an_alias') as bts_session:
        bts_session.rob_rzeczy()

1) przy tworzenu wola sie to 
2) pozniej wola sie  ___enter__
3) przed usunieciem obiektu wola sie __exit__


#czyli mamy definicje obiektu klasy ktora moze byc wykorzystana w with
#oto ona :

------------------------------------
import session

class Session(object):
    def __init__(self, library, setup_method, teardown_method, alias="default"):
        self.lib_instance = session.get_module_instance(library)
        self.session_setup = setup_method
        self.session_teardown = teardown_method
        self.alias = alias

    def __enter__(self):
        getattr(self.lib_instance, self.session_setup)(alias=self.alias)
        return self.lib_instance

    def __exit__(self, exc_type, exc_val, exc_tb):
        getattr(self.lib_instance, self.session_teardown)(alias=self.alias)


przy enterze zwracany jest         return self.lib_instance

ktory tworzony jest w session.get_module_instance(library)

tak wyglada modul session :

from importlib import import_module


def load_module(name):
    module_name = 'libraries.' + name
    return import_module(module_name)


def get_module_instance(name):
    return getattr(load_module(name), name)()

## Zatem klasa Session uzyta w klasie BtsSession 
zwroci obiekt biblioteki   ta_btssm_operations

I DOPIERO NA NIEJ ZAWOLA !!   reset_bts

a poniewaz jest to biblioteka musimy ja zinstancjonowac :

    bts_operation_mock_instance = bts_operation_mock.return_value

i sprawdzic czy zawolala sie odpowiednia metoda 

co sprawdzamy     bts_operation_mock_instance.reset_bts.assert_called_once()

## info Arka dotyczace importowania i uzywania mockow
Jeśli importujemy z użyciem „from”, to przy patchowaniu trzeba podać ścieżkę, gdzie dana klasa jest zaimportowana, 
nie tą, gdzie jest zdefiniowana.

1) Przykład patchowania zwykłego importa:

# plik a.py
import foo

# plik test_a.py
@patch(‘foo.bar’)
def test1(self, fooBarMock):



2) Przykład patchowania importa z “from”:

# plik a.py
from foo import bar

# plik test_a.py
@patch(‘a.bar’)
def test1(self, fooBarMock):



### PYTEST FIXTURE
#pytest fixture - jak to dziala ?
# dekorator ktory magicznie sprawia ze to co jest zwracane w dekorowanej funkcji jest dostepne w tescie

@pytest.fixture
def expected_calls_for_default_alias():
    return expected_calls('default')


@pytest.fixture
def expected_calls_for_specified_alias():
    return expected_calls('an_alias')


def expected_calls(alias):
    return [call(), call().setup_btssm(alias=alias), call().teardown_btssm(alias=alias)]