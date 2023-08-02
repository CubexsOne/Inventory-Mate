import { connect, MqttClient } from 'mqtt'
import logger from '../utils/logger'

interface Props {
  host: string
  port: number
  username: string
  password: string
  readTopic: string
  writeTopic: string
  callback: CallbackFunction
}

export type CallbackFunction = (barcode: string) => Promise<string>

export class MQTTClient {
  public static Instance: MQTTClient | null = null

  private client: MqttClient | undefined
  private readonly host: string
  private readonly port: number
  private readonly username: string
  private readonly password: string
  private readonly readTopic: string
  private readonly writeTopic: string
  private readonly callback: CallbackFunction

  constructor(props: Props) {
    MQTTClient.Instance = this

    this.host = props.host
    this.port = props.port
    this.username = props.username
    this.password = props.password
    this.readTopic = props.readTopic
    this.writeTopic = props.writeTopic
    this.callback = props.callback

    this.connect()
  }

  private connect(): void {
    this.client = connect(`mqtt://${this.host}:${this.port}`, {
      clean: true,
      clientId: `mqtt_bridge_${Math.random().toString(16).slice(3)}`,
      encoding: 'utf8',
      username: this.username,
      password: this.password,
      reconnectPeriod: 5 * 1000,
    })

    this.client.once('connect', () => {
      logger.info('MQTT-Client connected 🟢')
      this.client?.subscribe(this.readTopic)
    })

    this.client?.on('message', async (topic, message) => {
      if (topic.toLowerCase() !== this.readTopic.toLowerCase()) return
      const stringifiedMessage = message.toString().trim()
      if (stringifiedMessage.length === 0) return

      const response = await this.callback(message.toString().toLowerCase())
      this.sendMessage(response)
    })
  }

  private sendMessage(message: string): void {
    if (this.client?.connected) {
      this.client.publish(this.writeTopic, message)
    }
  }
}
