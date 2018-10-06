import { Component } from '@angular/core';
import { NavController, Button } from 'ionic-angular';
import { BluetoothSerial } from '@ionic-native/bluetooth-serial';
import { AlertController } from 'ionic-angular';
import { ClassStmt } from '@angular/compiler';

@Component({
  selector: 'page-home',
  templateUrl: 'home.html'
})

export class HomePage {
  //colores iniciales
  clase00 = 'light';
  clase10 = 'light';
  clase20 = 'light';
  clase30 = 'light';
  clase40 = 'light';
  clase01 = 'light';
  clase11 = 'light';
  clase21 = 'light';
  clase31 = 'light';
  clase41 = 'light';
  clase02 = 'light';
  clase12 = 'light';
  clase22 = 'light';
  clase32 = 'light';
  clase42 = 'light';

  unpairedDevices: any;
  pairedDevices: any;
  gettingDevices: Boolean;

  constructor(private bluetoothSerial: BluetoothSerial, private alertCtrl: AlertController) {
    bluetoothSerial.enable();
  }

  starScanning() {
    this.pairedDevices = null;
    this.unpairedDevices = null;
    this.gettingDevices = true;
    this.bluetoothSerial.discoverUnpaired().then((success) => {
      this.unpairedDevices = success;
      this.gettingDevices = false;
      success.forEach(element => {
        //alert(element.name);
      });
    },
      (err) => {
        console.log(err);
      })

    this.bluetoothSerial.list().then((success) => {
      this.pairedDevices = success;
    },
      (err) => {

      })
  }
  success = (data) => alert(data);
  fail = (error) => alert(error);

  selectDevice(address: any) {

    let alert = this.alertCtrl.create({
      title: 'Connectar',
      message: 'Quiere conectarse con?',
      buttons: [
        {
          text: 'Cancelar',
          role: 'cancelar',
          handler: () => {
            console.log('Cancel clicked');
          }
        },
        {
          text: 'Connectar',
          handler: () => {
            this.bluetoothSerial.connect(address).subscribe(this.success, this.fail);
          }
        }
      ]
    });
    alert.present();
  }

  disconnect() {
    let alert = this.alertCtrl.create({
      title: 'Desconectar?',
      message: 'Quiere desconectarse?',
      buttons: [
        {
          text: 'Cancelar',
          role: 'cancelar',
          handler: () => {
            console.log('Cancel clicked');
          }
        },
        {
          text: 'Desconectar',
          handler: () => {
            this.bluetoothSerial.disconnect();
          }
        }
      ]
    });
    alert.present();
  }

  btCalibrar() {
    let alert = this.alertCtrl.create({
      title: 'Calibrar',
      message: 'Quiere calibrar la maquina',
      buttons: [
        {
          text: 'Cancelar',
          role: 'cancelar',
          handler: () => {
            console.log('Cancel clicked');
          }
        },
        {
          text: 'Calibrar',
          handler: () => {
            this.bluetoothSerial.write('Calibrar');
            console.log('calibrando...');
          }
        }
      ]
    });
    alert.present();
  }

  btCoordenada($valor) {
    this.bluetoothSerial.write($valor);
    console.log($valor);

    switch ($valor) {
      case '00':
        this.clase00 = 'secondary';
        break;
      case '10':
        this.clase10 = 'secondary';
        break;
      case '20':
        this.clase20 = 'secondary';
        break;
      case '30':
        this.clase30 = 'secondary';
        break;
      case '40':
        this.clase40 = 'secondary';
        break;
      case '01':
        this.clase01 = 'secondary';
        break;
      case '11':
        this.clase11 = 'secondary';
        break;
      case '21':
        this.clase21 = 'secondary';
        break;
      case '31':
        this.clase31 = 'secondary';
        break;
      case '41':
        this.clase41 = 'secondary';
        break;
      case '02':
        this.clase02 = 'secondary';
        break;
      case '12':
        this.clase12 = 'secondary';
        break;
      case '22':
        this.clase22 = 'secondary';
        break;
      case '32':
        this.clase32 = 'secondary';
        break;
      case '42':
        this.clase42 = 'secondary';
        break;
    }

  }

}
